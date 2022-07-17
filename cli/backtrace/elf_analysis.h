/// 
/// \file: elf_analysis.h
/// \author: GrandChris
/// \date: 2021-03-18
/// \brief: Decleration of the result of the analysis of an .elf file
///

#include <string>
#include <vector>
#include <unordered_map>

#pragma once

/// 
/// \class   Line
/// \brief   The description of a line in a source file
/// \author  GrandChris
/// \date    2021-03-18
/// 
struct Line {
    std::string filename;   // The name of the source file
    std::string path;       // The path to the source file
    uint32_t line;          // The line number inside the source file
    uint32_t column;        // The column number inside the source file
    bool isStartSequence;   // If the line number is the start of a function
    uint64_t address;
};

/// 
/// \class   DisassembledLine
/// \brief   A disassembled opcode with info of the corresponding source file
/// \author  GrandChris
/// \date    2021-03-18
/// 
struct DisassembledLine {
    uint64_t address;               // The program address of the opcode
    std::string opcode_description; // opcode as mnemonic and operands
    uint64_t branch_destination;    // The program address to the destination of a branch instruction
    Line branch_destination_line;   // The description of a line in a source file of the destination of a branch instruction
    Line line;                      // The description of a line in a source file
};

/// 
/// \class   DisassembledFile
/// \brief   A struct with a line for every opcode in the .text section
/// \author  GrandChris
/// \date    2021-03-18
/// 
struct DisassembledFile {
    std::string filename;    // The name of the disassembled .elf file (if available)
    std::vector<DisassembledLine> lines; // A line for every opcode in the .text section
    std::unordered_map<uint64_t, Line> addr_lines; // A line for every addr
    uint64_t rx_vaddr;
};


//////////////////////////////////////////////////////
// Function Declearations

DisassembledFile disassembleFile(std::string const & filename);
DisassembledFile disassembleInput(std::istream & in);