#define _POSIX_C_SOURCE 200112L
#include <coredumpparser.hpp>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <filesystem>
#include "zstr/zstr.hpp"

#include "elfio/elfio.hpp"
#include "dwarf/debug_line/header.h"
#include "dwarf/debug_line/state_machine.h"
#include "elf_analysis.h"

namespace fs = std::filesystem;

#define DUMP_DEC_FORMAT( width ) \
    std::setw( width ) << std::setfill( ' ' ) << std::dec << std::right
#define DUMP_DEC0_FORMAT( width ) \
    std::setw( width ) << std::setfill( '0' ) << std::dec << std::right
#define DUMP_HEX0x_FORMAT( width ) \
    "0x" << std::setw( width ) << std::setfill( '0' ) << std::hex << std::right
#define DUMP_HEX_FORMAT( width ) \
    std::setw( width ) << std::setfill( '0' ) << std::hex << std::right
#define DUMP_STR_FORMAT( width ) \
    std::setw( width ) << std::setfill( ' ' ) << std::hex << std::left


void print_addr2line(uint32_t addr, DisassembledFile* res, vita::coredump::parser* parser)
{
    auto moduleinfo = parser->getModuleInfo();
    bool located = false;
    if (moduleinfo)
    {
        for (auto module: moduleinfo->modules())
        {
            for (auto segment: module.segments())
            {
                if ( addr >= segment.baseAddr() && addr <= (segment.baseAddr() + segment.memorySize()))
                {
                    located = true;
//                    std::cout << "located" << std::endl;
                    addr -= segment.baseAddr();
                    break;
                }
            }
            if (located) break;
        }
        if (located)
        {
//            std::cout << DUMP_HEX0x_FORMAT(8) << addr << std::endl;
//            std::cout << DUMP_HEX0x_FORMAT(8) << res->rx_vaddr << std::endl;
            addr += res->rx_vaddr;
            addr &= ~1;
//            std::cout << DUMP_HEX0x_FORMAT(8) << addr << std::endl;
            auto line = res->addr_lines.find(addr);
            if (line != res->addr_lines.end())
            {
                std::cout << "         => "  << line->second.path << "/" << line->second.filename << " at " << std::dec << line->second.line << ":" << line->second.column  << std::endl;
            }
        }
    }


}

void cat_stream(std::istream& is, std::ostream& os)
{
    const std::streamsize buff_size = 1 << 16;
    char * buff = new char [buff_size];
    while (true)
    {
        is.read(buff, buff_size);
        std::streamsize cnt = is.gcount();
        if (cnt == 0) break;
        os.write(buff, cnt);
    }
    delete [] buff;
}

void decompress_file(const std::string& in, const std::string& out)
{
    std::unique_ptr< std::ofstream > ofs_p;
    if (out.empty())
    {
        std::cout << "WTF" << std::endl;
        return;
    }
    ofs_p = std::unique_ptr< std::ofstream >(new strict_fstream::ofstream(out));
    std::ostream * os_p = ofs_p.get();

    std::unique_ptr< std::istream > is_p = std::unique_ptr< std::istream >(new zstr::ifstream(in));
    cat_stream(*is_p, *os_p);
}

int main( int argc, char** argv )
{
    if ( argc < 2 ) {
        printf( "Usage: backtrace <file_name> [elf_name]\n" );
        return 1;
    }

    std::string outname = fs::temp_directory_path() / "coredump.tmp";

    decompress_file(argv[1], outname);

    vita::coredump::parser parser(outname);

    if ( !parser.parse() ) {
        printf( "File %s is not found or it is not an PSP2 coredump file\n", argv[1] );
        return 1;
    }

    DisassembledFile res;

    if (argc > 2) {
        std::string elf_name = argv[2];
        res = disassembleFile(elf_name);
        for (auto line: res.addr_lines)
            {
//                std::cout << DUMP_HEX0x_FORMAT(8) << line.first << ": " << line.second.path << "/" << line.second.filename << " at " << std::dec << line.second.line << ":" << line.second.column  << std::endl;
            }
    }

    auto stackinfo = parser.getStackInfo();
    if (stackinfo)
    {
//      j["stack_use"] = stackinfo->stacks();
    }

    auto threadinfo = parser.getThreadInfo();
    if (threadinfo)
    {
        std::vector<vita::coredump::threadInfoThread> _crashed;
        for(auto& thread: threadinfo->threads())
        {
            std::string name = thread.name();
            if (name.empty()) {
                name = "<UNNAMED>";
            }
            std::cout << "Thread " << DUMP_HEX0x_FORMAT(8) << thread.id() << ": " << name << std::endl;
            std::cout << "  Status: " << thread.readableStatus() << std::endl;
            if (thread.stopReason() != 0)
            {
                std::cout << "  Stop reason: " << thread.readableStopReason() << std::endl;
                std::cout << "  PC: " << DUMP_HEX0x_FORMAT(8) << thread.PC() << std::endl;
                _crashed.push_back(thread);
            }
            std::cout << std::endl;
        }

        std::cout << "-------------------------------------------------------------------------------------\n\n";

        auto threadreginfo = parser.getThreadRegInfo();
        auto reader = parser.getReader();

        for(auto& thread: _crashed)
        {
            std::string name = thread.name();
            if (name.empty()) {
                name = "<UNNAMED>";
            }
            std::cout << "Thread: '" << name << "' (" << DUMP_HEX0x_FORMAT(8) << thread.id() << ") crashed: " << thread.readableStopReason() << std::endl;
            if (!res.filename.empty())
            {
                uint32_t addr = thread.PC() & ~1;

                bool pc_located = false;
                auto moduleinfo = parser.getModuleInfo();
                if (moduleinfo) {
                    for (auto module: moduleinfo->modules())
                    {
                        for (auto segment: module.segments())
                        {
                            if ( addr >= segment.baseAddr() && addr <= (segment.baseAddr() + segment.memorySize()))
                            {
                                pc_located = true;
                                addr -= segment.baseAddr();
                                break;
                            }
                        }
                        if (pc_located) break;
                    }
                }

                if (pc_located) {
                    addr += res.rx_vaddr;
                    auto line = res.addr_lines.find(addr);
                    if (line != res.addr_lines.end())
                    {
                        std::cout << line->second.path << "/" << line->second.filename << " at " << std::dec << line->second.line << ":" << line->second.column  << std::endl;
                    }
                } else {
                    if (threadreginfo)
                    {
                        auto regs = threadreginfo->threadRegs().find(thread.id());
                        if (regs != threadreginfo->threadRegs().end())
                        {
                            uint32_t addr = regs->second.registers()[14] & ~1;

                            bool lr_located = false;
                            auto moduleinfo = parser.getModuleInfo();
                            if (moduleinfo) {
                                for (auto module: moduleinfo->modules())
                                {
                                    for (auto segment: module.segments())
                                    {
                                        if ( addr >= segment.baseAddr() && addr <= (segment.baseAddr() + segment.memorySize()))
                                        {
                                            lr_located = true;
                                            addr -= segment.baseAddr();
                                            break;
                                        }
                                    }
                                    if (lr_located) break;
                                }
                            }

                            if (lr_located) {
                                addr += res.rx_vaddr;
                                auto line = res.addr_lines.find(addr);
                                if (line != res.addr_lines.end())
                                {
                                    std::cout << line->second.path << "/" << line->second.filename << " at " << std::dec << line->second.line << ":" << line->second.column  << std::endl;
                                }
                            }
                        }
                    }

                }
            }

            if (threadreginfo)
            {
                auto regs = threadreginfo->threadRegs().find(thread.id());
                if (regs != threadreginfo->threadRegs().end())
                {
                    std::cout << std::endl << "Thread registers:" << std::endl;
                    std::cout << "CPSR:  " << DUMP_HEX0x_FORMAT(8) << regs->second.CPSR()
                    << "    TPIDRURO: " << DUMP_HEX0x_FORMAT(8) << regs->second.TPIDRURO()
                    << std::endl;

                    std::cout << "FPSCR: " << DUMP_HEX0x_FORMAT(8) << regs->second.FPSCR()
                    << "    TPIDRURW: " << DUMP_HEX0x_FORMAT(8) << regs->second.TPIDRURW()
                    << std::endl;

                    std::cout << "CPACR: " << DUMP_HEX0x_FORMAT(8) << regs->second.CPACR()
                    << "    FPEXC:    " << DUMP_HEX0x_FORMAT(8) << regs->second.FPEXC()
                    << std::endl;

                    std::cout << "DACR:  " << DUMP_HEX0x_FORMAT(8) << regs->second.DACR()
                    << "    DBGDSCR:  " << DUMP_HEX0x_FORMAT(8) << regs->second.DBGDSCR()
                    << std::endl;

                    std::cout << "IFSR:  " << DUMP_HEX0x_FORMAT(8) << regs->second.IFSR()
                    << "    DFSR:     " << DUMP_HEX0x_FORMAT(8) << regs->second.DFSR()
                    << std::endl;

                    std::cout << "IFAR:  " << DUMP_HEX0x_FORMAT(8) << regs->second.IFAR()
                    << "    DFAR:     " << DUMP_HEX0x_FORMAT(8) << regs->second.DFAR()
                    << std::endl;

                    for (int i = 0; i < 4; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            if (i*4+j == 13)
                                std::cout << "SP : " << DUMP_HEX0x_FORMAT(8) << regs->second.registers()[i*4+j] << "    ";
                            else if (i*4+j == 14)
                                std::cout << "LR : " << DUMP_HEX0x_FORMAT(8) << regs->second.registers()[i*4+j] << "    ";
                            else if (i*4+j == 15)
                                std::cout << "PC : " << DUMP_HEX0x_FORMAT(8) << regs->second.registers()[i*4+j] << "    ";
                            else
                            std::cout << "r" << DUMP_DEC0_FORMAT(2) << (i*4+j) << ": " << DUMP_HEX0x_FORMAT(8) << regs->second.registers()[i*4+j] << "    ";
                        }
                        std::cout << std::endl;
                    }

                    for (int i = 0; i < 8; i++)
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            std::cout << "neon" << DUMP_DEC0_FORMAT(2) << (i*8+j) << ": " << DUMP_HEX0x_FORMAT(16) << regs->second.neon()[i*8+j] << "    ";
                        }
                        std::cout << std::endl;
                    }

                    for (auto seg: reader->segments)
                    {
                        uint32_t sp_addr = regs->second.registers()[13];
                        if (
                            sp_addr >= seg->get_virtual_address()
                            && sp_addr <= (seg->get_virtual_address() + seg->get_memory_size())
                        )
                        {
                            std::cout << std::endl << "Stack contents around SP:" << std::endl;
                            const uint32_t* data = (const uint32_t*)seg->get_data();

                            for (int addr = -16; addr < 16; addr++)
                            {
                                uint32_t vaddr = addr*4 + sp_addr;
                                if (vaddr == sp_addr)
                                    std::cout << "SP-> " << DUMP_HEX0x_FORMAT(8) << vaddr << ": " << DUMP_HEX0x_FORMAT(8) << data[(vaddr- seg->get_virtual_address())/4] << std::endl;
                                else
                                    std::cout << "     " <<  DUMP_HEX0x_FORMAT(8) << vaddr << ": " << DUMP_HEX0x_FORMAT(8) << data[(vaddr- seg->get_virtual_address())/4] << std::endl;
                                print_addr2line(data[(vaddr- seg->get_virtual_address())/4], &res, &parser);
                            }
                            break;
                        }
                    }

                }
            }

        }

    }


    fs::remove(outname);

    return 0;
}
