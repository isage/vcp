/// 
/// File:   header.h
/// Author: GrandChris
/// Date:   2021-03-18
/// Brief:  The header of the dwarf debug_line section
///

#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <span>
#include <string>
#include <vector>

namespace dwarf {
    namespace debug_line {
        struct Header
        {
            // The size in bytes of the line number information for this compilation unit, not
            // including the length field itself (see Section 7.2.2 on page 184).
            uint32_t uint_length;

            //  A version number (see Section 7.22 on page 236). This number is specific to
            //  the line number information and is independent of the DWARF version
            //  number.
            uint16_t version;

            // A 1-byte unsigned integer containing the size in bytes of an address (or offset
            // portion of an address for segmented addressing) on the target system.
            // The address_size field is new in DWARF Version 5. It is needed to support the
            // common practice of stripping all but the line number sections (.debug_line and
            // .debug_line_str) from an executable.
            // uint8_t address_size;

            // A 1-byte unsigned integer containing the size in bytes of a segment selector
            // on the target system.
            // The segment_selector_size field is new in DWARF Version 5. It is needed in
            // combination with the address_size field to accurately characterize the address
            // representation on the target system.
            // uint8_t segment_selector_size;

            // The number of bytes following the header_length field to the beginning of
            // the first byte of the line number program itself. In the 32-bit DWARF format,
            // this is a 4-byte unsigned length; in the 64-bit DWARF format, this field is an
            // 8-byte unsigned length (see Section 7.4 on page 196).
            uint32_t header_length;

            // The size in bytes of the smallest target machine instruction. Line number
            // program opcodes that alter the address and op_index registers use this and
            // maximum_operations_per_instruction in their calculations.
            uint8_t minimum_instruction_length;

            // The maximum number of individual operations that may be encoded in an
            // instruction. Line number program opcodes that alter the address and
            // op_index registers use this and minimum_instruction_length in their
            // calculations.
            // For non-VLIW architectures, this field is 1, the op_index register is always 0,
            // and the operation pointer is simply the address register.
            uint8_t maximum_operations_per_instruction;

            // The initial value of the is_stmt register.
            // A simple approach to building line number information when machine instructions
            // are emitted in an order corresponding to the source program is to set
            // default_is_stmt to “true” and to not change the value of the is_stmt register
            // within the line number program. One matrix entry is produced for each line that has
            // code generated for it. The effect is that every entry in the matrix recommends the
            // beginning of each represented line as a breakpoint location. This is the traditional
            // practice for unoptimized code.
            // A more sophisticated approach might involve multiple entries in the matrix for a line
            // number; in this case, at least one entry (often but not necessarily only one) specifies a
            // recommended breakpoint location for the line number. DW_LNS_negate_stmt
            // opcodes in the line number program control which matrix entries constitute such a
            // recommendation and default_is_stmt might be either “true” or “false.” This
            // approach might be used as part of support for debugging optimized code.
            uint8_t default_is_stmt;

            // This parameter affects the meaning of the special opcodes. See below
            int8_t line_base;

            // This parameter affects the meaning of the special opcodes. See below
            uint8_t line_range;

            // The number assigned to the first special opcode.
            // Opcode base is typically one greater than the highest-numbered standard opcode
            // defined for the specified version of the line number information (12 in DWARF
            // Versions 3, 4 and 5, and 9 in Version 2). If opcode_base is less than the typical value,
            // then standard opcode numbers greater than or equal to the opcode base are not used
            // in the line number table of this unit (and the codes are treated as special opcodes). If
            // opcode_base is greater than the typical value, then the numbers between that of the
            // highest standard opcode and the first special opcode (not inclusive) are used for
            // vendor specific extensions.
            uint8_t opcode_base;

            // This array specifies the number of LEB128 operands for each of the standard
            // pcodes. The first element of the array corresponds to the opcode whose
            // alue is 1, and the last element corresponds to the opcode whose value is
            // pcode_base - 1.
            // y increasing opcode_base, and adding elements to this array, new standard
            // pcodes can be added, while allowing consumers who do not know about these new
            // pcodes to be able to skip them.
            // odes for vendor specific extensions, if any, are described just like standard opcodes.
            // The remaining fields provide information about the source files used in the compilation.
            // These fields have been revised in DWARF Version 5 to support these goals:
            // • To allow new alternative means for a consumer to check that a file it can access is
            // the same version as that used in the compilation.
            // • To allow a producer to collect file name strings in a new section
            // (.debug_line_str) that can be used to merge duplicate file name strings.
            // • To add the ability for producers to provide vendor-defined information that can be
            // skipped by a consumer that is unprepared to process it.
            uint8_t standard_opcode_lengths[12];

            // The sequence contains an entry for each path that was searched for included source files in 
            // this compilation. (The paths include those directories specified explicitly by the user for the 
            // compiler to search and those the compiler searches without explicit direction). Each path 
            // entry is either a full path name or is relative to the current directory of the compilation. The 
            // current directory of the compilation is understood to be the first entry and is not explicitly 
            // represented. Each entry is a null-terminated string containing a full path name. The last entry 
            // is followed by a single null byte. 
            std::vector<std::string> include_directories;

            struct file_name {
                std::string name;
                uint64_t include_directories_index;
                uint64_t time_last_modified;
                uint64_t size;
            };

            // The sequence contains an entry for each source file that contributed to the line number 
            // information for this compilation unit or is used in other contexts, such as in a declaration 
            // coordinate or a macro file inclusion. Each entry consists of the following values: 
            // • A null-terminated string containing the file name. 
            // • An unsigned LEB128 number representing the directory index of the directory in which 
            // the file was found. 
            // • An unsigned LEB128 number representing the (implementation-defined) time of last 
            // modification for the file. 
            // • An unsigned LEB128 number representing the length in bytes of the file. 
            // A compiler may choose to emit LEB128(0) for the time and length fields to indicate that this 
            // information is not available. The last entry is followed by a single null byte. 
            // The directory index represents an entry in the include_directories section. The index is 
            // LEB128(0) if the file was found in the current directory of the compilation, LEB128(1) if it 
            // was found in the first directory in the include_directories section, and so on. The 
            // directory index is ignored for file names that represent full path names. 
            std::vector<file_name> file_names;


            // the data following the header
            std::span<uint8_t const> data_section;

            ///////////////////////////////////////////////////////////////////
            // public functions
            
            void print(std::ostream & ost) const;

            size_t size() const;

            ///////////////////////////////////////////////////////////////////
            // static functions
            
            static Header read_one(std::span<uint8_t const> data);
            static std::vector<Header> read(std::span<uint8_t const> data);
        };




///////////////////////////////////////////////////////////////////
// implementation

    } // debug_line
} // dwarf