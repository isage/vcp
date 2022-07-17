/// 
/// \file: elf_analysis.cpp
/// \author: GrandChris
/// \date: 2021-03-18
/// \brief: Analyses a .elf file
///

#include "elf_analysis.h"
#include "dwarf/debug_line/header.h"
#include "dwarf/debug_line/state_machine.h"
#include "elfio/elfio.hpp"
#include <chrono>
#include <filesystem>
#include <sstream>
#include <unordered_map>

#include "capstone/capstone.h"

DisassembledFile disassembleFile(std::string const & filename) 
{
    std::ifstream stream;
        stream.open( filename.c_str(), std::ios::in | std::ios::binary );
        if ( !stream ) {
            return {};
        }
    DisassembledFile res = disassembleInput(stream);
    res.filename = filename;

    return res;
}

DisassembledFile disassembleInput(std::istream & in) 
{
    DisassembledFile res = {};
    res.filename = "";

    // Create elfio reader
    ELFIO::elfio reader; 
    if ( !reader.load( in ) ) {
            std::cout << "Can't find or process ELF file " << std::endl;
    }

    // read sections
    std::span<uint8_t const> debug_line;
    std::span<uint8_t const> text;

    for (auto seg: reader.segments)
    {
        if (seg->get_type() == ELFIO::PT_LOAD && ( seg->get_flags() == (ELFIO::PF_R | ELFIO::PF_X)))
        {
            res.rx_vaddr = seg->get_virtual_address();
        }
    }

    for ( auto psec: reader.sections)
    {
        if(psec->get_name() == ".debug_line")
        {
            debug_line =  std::span<uint8_t const>(reinterpret_cast<uint8_t const *>(psec->get_data()), static_cast<size_t>(psec->get_size()));
        }
    }

    // read debug_line headers
    auto const debug_line_headers = dwarf::debug_line::Header::read(debug_line);

    for(auto const & header : debug_line_headers) 
    {
        auto const lineTable = dwarf::debug_line::decode_data(header);

        for(auto const & elem : lineTable) {
            if(!elem.end_sequence) {
                auto const address = elem.address;
                auto const & fileName = header.file_names[elem.file - 1];

                if (fileName.include_directories_index != 0)
                {
                    Line lineMapData = {
                        .filename = std::string(fileName.name),
                        .path = std::string(header.include_directories.at(fileName.include_directories_index - 1)),
                        .line = elem.line,
                        .column = elem.column,
                        .address = address
                    };

                    res.addr_lines.insert({address, lineMapData});
                }
            }
        }

    }

    return res;
}
