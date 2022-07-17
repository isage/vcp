/// 
/// File:   header.cpp
/// Author: GrandChris
/// Date:   2021-03-18
/// Brief:  The header of the dwarf debug_line section
///

#include "debug_line/header.h"
#include "leb128.h"
#include <cassert>
#include <iomanip>
#include <cstring>

using namespace dwarf::debug_line;

///
/// \brief  Prints the line table
/// \author GrandChris
/// \date   2021-03-18
///
void Header::print(std::ostream & ost) const
{
    using namespace std;

    ost << "Dump of section .debug_line:" << endl;
    ost << endl;

    ost << setw(28) << left << "Length: " << uint_length << endl;
    ost << setw(28) << left << "DWARF Version: " << version << endl;
    ost << setw(28) << left << "Prologue Length: " << header_length << endl;
    ost << setw(28) << left << "Minimum Instruction Length: " << static_cast<uint32_t>(minimum_instruction_length) << endl;
    ost << setw(28) << left << "Initial value of 'is_stmt': " << static_cast<uint32_t>(default_is_stmt) << endl;
    ost << setw(28) << left << "Line Base: " << static_cast<int32_t>(line_base) << endl;
    ost << setw(28) << left << "Line Range: " << static_cast<uint32_t>(line_range) << endl;
    ost << setw(28) << left << "Opcode Base: " << static_cast<uint32_t>(opcode_base) << endl;
    ost << endl;

    ost << "Opcodes" << endl;

    for(size_t i = 0; i < std::size(standard_opcode_lengths); ++i) {
        ost << "Opcode " << i << " has " << static_cast<uint32_t>(standard_opcode_lengths[i]) << " args" << endl;
    }
    ost << endl;

    ost << "The Dictionary Table:" << endl;
    for(size_t i = 0; i < include_directories.size(); ++i) {
        ost << setw(3) << left << i+1 << " " << include_directories[i] << endl;
    }
    ost << endl;

    ost << "The File Name Table:" << endl;
    ost << "Entry Dir Time Size Name" << endl;
    for(size_t i = 0; i < file_names.size(); ++i) {
        ost << setw(3) << left << i+1 << " "  
        << setw(3) << left << file_names[i].include_directories_index << " " 
        << setw(3) << left << file_names[i].time_last_modified << " " 
        << setw(3) << left << file_names[i].size << " " 
        <<  file_names[i].name << endl;
    }
    ost << endl;
}

///
/// \brief Returns the size of the header (including the following data)
/// \author GrandChris
/// \date 2021-03-18
///
size_t Header::size() const 
{
    return uint_length + sizeof(uint_length);
}

///
/// \brief Parses the header from the '.debug_line' section
/// \author GrandChris
/// \date 2021-03-18
/// \param data Binary array where the first byte is the start of a header in the '.debug_line' section 
/// \return  The decoded header
///
Header Header::read_one(std::span<uint8_t const> data) 
{
    Header res = {};
    int i = 0;

    memcpy(&res.uint_length, &data[i], sizeof(res.uint_length)); i += sizeof(res.uint_length);
    memcpy(&res.version, &data[i], sizeof(res.version));  i += sizeof(res.version);
    
    if(res.version != 3) {
        std::cout << "Warning: .debug_line section with header version " << res.version << " detected (only version 3 is supported)" << std::endl;
        return res;
    }
    assert(res.version == 3);
    memcpy(&res.header_length, &data[i], sizeof(res.header_length));  i += sizeof(res.header_length);
    memcpy(&res.minimum_instruction_length, &data[i], sizeof(res.minimum_instruction_length));  i += sizeof(res.minimum_instruction_length);   
    memcpy(&res.default_is_stmt, &data[i], sizeof(res.default_is_stmt));  i += sizeof(res.default_is_stmt);
    memcpy(&res.line_base, &data[i], sizeof(res.line_base));  i += sizeof(res.line_base);
    memcpy(&res.line_range, &data[i], sizeof(res.line_range));  i += sizeof(res.line_range);
    memcpy(&res.opcode_base, &data[i], sizeof(res.opcode_base));  i += sizeof(res.opcode_base);

    // read opcodes
    assert(res.opcode_base == 13);
    for(size_t j = 0; j < std::size(res.standard_opcode_lengths); ++j) {
        memcpy(&res.standard_opcode_lengths[j], &data[i], sizeof(res.standard_opcode_lengths[j])); i += sizeof(res.standard_opcode_lengths[i]);
    }

    // read direcories
    while(data[i] != 0) {
        res.include_directories.push_back(std::string(reinterpret_cast<char const *>(&data[i])));
        i += res.include_directories.back().size() + 1;
    }
    i += 1;

    // read filenames
   while(data[i] != 0) {
        Header::file_name entry = {};
        entry.name = std::string(reinterpret_cast<char const *>(&data[i]));
        i += entry.name.size()+1;

        i += decodeUleb128(data.subspan(i), entry.include_directories_index);
        i += decodeUleb128(data.subspan(i), entry.time_last_modified);
        i += decodeUleb128(data.subspan(i), entry.size);

        res.file_names.push_back(entry);        
    }    
    i += 1;

    // store pointer to the data section
   

    // sanity check
    size_t const header_size = i;
    size_t const calculated_header_size = res.header_length + sizeof(header_length) + sizeof(version) + sizeof(uint_length);
    assert(header_size == calculated_header_size);

    size_t const data_size = res.uint_length + sizeof(uint_length) - header_size;
    // size_t const data_size2 = data.size();
    // size_t const current_size = res.data_section.size();

    res.data_section = std::span(&data[i], data_size); // data.subspan(i);

    //assert(res.data_section.size() == data_size);

    return res;
}


///
/// \brief Parses all the headers from the '.debug_line' section
/// \author GrandChris
/// \date 2021-03-18
/// \param data Binary array where the first byte is the start of a header in the '.debug_line' section 
/// \return  The decoded headers
///
std::vector<Header> Header::read(std::span<uint8_t const> data) 
{
    std::vector<Header> res;

    size_t i = 0;
    while(i < data.size()) {
        auto const header = read_one(data.subspan(i));
        res.push_back(header);
        i += header.size();
        // header.print(std::cout);
    }

    assert(i == data.size());

    return res;
}
