// 
// File: state_machine_register.h
// Author: GrandChris
// Date: 2021-03-18
// Brief: Registers for the dwarf parse line state machine
//

#pragma once

#include <cstdint>

namespace dwarf {
    namespace debug_line {
        /// 
        /// \class   StateMachineRegisters
        /// \brief   Registers of the DWARF 'debug_line' state machine
        /// \author  GrandChris
        /// \date    2021-03-18
        /// 
        struct StateMachineRegisters {
            // The program-counter value corresponding to a
            // machine instruction generated by the compiler.
            uint64_t address;

            // An unsigned integer representing the index of
            // an operation within a VLIW instruction. The
            // index of the first operation is 0. For non-VLIW
            // architectures, this register will always be 0.    
            uint32_t op_index;

            // An unsigned integer indicating the identity of
            // the source file corresponding to a machine
            // instruction
            uint32_t file;

            // An unsigned integer indicating a source line
            // number. Lines are numbered beginning at 1.
            // The compiler may emit the value 0 in cases
            // where an instruction cannot be attributed to any
            // source line.
            uint32_t line;

            // An unsigned integer indicating a column
            // number within a source line. Columns are
            // numbered beginning at 1. The value 0 is
            // reserved to indicate that a statement begins at
            // the “left edge” of the line.
            uint32_t column;

            // A boolean indicating that the current instruction
            // is a recommended breakpoint location. A
            // recommended breakpoint location is intended
            // to “represent” a line, a statement and/or a
            // semantically distinct subpart of a statement.
            bool is_stmt;

            // A boolean indicating that the current instruction
            // is the beginning of a basic block.
            bool basic_block;

            // A boolean indicating that the current address is
            // that of the first byte after the end of a sequence
            // of target machine instructions. end_sequence
            // terminates a sequence of lines; therefore other
            // information in the same row is not meaningful.    
            bool end_sequence;

            // A boolean indicating that the current address is
            // one (of possibly many) where execution should
            // be suspended for a breakpoint at the entry of a
            // function,
            bool prologue_end;

            // A boolean indicating that the current address is
            // one (of possibly many) where execution should
            // be suspended for a breakpoint just prior to the
            // exit of a function
            bool epilogue_begin;

            // An unsigned integer whose value encodes the
            // applicable instruction set architecture for the
            // current instruction.
            // The encoding of instruction sets should be shared by
            // all users of a given architecture. It is recommended
            // that this encoding be defined by the ABI authoring
            // committee for each architecture.
            uint32_t isa;

            // An unsigned integer identifying the block to
            // which the current instruction belongs.
            // Discriminator values are assigned arbitrarily by
            // the DWARF producer and serve to distinguish
            // among multiple blocks that may all be
            // associated with the same source file, line, and
            // column. Where only one block exists for a given
            // source position, the discriminator value is be
            // zero.
            uint32_t discriminator;
        };
    } // debug_line
} // dwarf

