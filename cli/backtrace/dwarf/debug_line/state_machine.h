/// 
/// \file: state_machine.h
/// \author: GrandChris
/// \date: 2021-03-18
/// \brief: The state machine to decode the line table
///

#pragma once

#include "header.h"
#include "state_machine_register.h"

namespace dwarf {
    namespace debug_line {

/// 
/// \class   StateMachine
/// \brief   The state machine to decode the line table
/// \author  GrandChris
/// \date    2021-03-18
/// \details http://dwarfstd.org/Download.php
///          Currently only support for 'Dwarf3'
///
class StateMachine {
public:

    //////////////////////////////////////////////////////
    // Constructor

    StateMachine(Header const & header);

    //////////////////////////////////////////////////////
    // Public Methods

    void process(std::span<uint8_t const> data);
    std::vector<StateMachineRegisters> getLineTable() const;
    
private:

    //////////////////////////////////////////////////////
    // Type Definitions

    struct SpecialOpcode {
        uint32_t address_increment;
        int32_t line_increment;
    };

    //////////////////////////////////////////////////////
    // Private Methods

    void reset();
    void specialOpcode(int32_t add_line, uint32_t add_address);
    void dw_lns_copy();
    void dw_lns_advance_pc(uint32_t leb128_operand);
    void dw_lns_advance_line(int32_t leb128_operand);
    void dw_lns_set_file(uint32_t leb128_operand);
    void dw_lns_set_column(uint32_t leb128_operand);
    void dw_lns_negate_stmt();
    void dw_lns_set_basic_block();
    void dw_lns_const_add_pc();
    void dw_lns_fixed_advance_pc(uint16_t operand);
    void dw_lns_set_prologue_end();
    void dw_lns_set_epilogue_begin();
    void dw_lns_set_isa(uint32_t leb128_operand);
    void dw_line_end_sequence();
    void dw_line_set_address(uint32_t address);
    void dw_line_define_file(char const * source_file_name, uint32_t leb128_directory_index,
        uint32_t leb128_last_time_modified, uint32_t leb128_file_length);
    void dw_lne_set_discriminator(uint32_t leb128_operand);
    
    bool isSpecialOpcode(uint8_t const opcode) const;
    SpecialOpcode decodeSpecialOpcode(uint8_t const opcode) const;
    uint32_t encodeOpcode(uint32_t const line_increment, uint32_t const address_increment) const;

    //////////////////////////////////////////////////////
    // Members

    Header mHeader;             // Header with info how to process the data
    StateMachineRegisters mReg; // Current state of the state machine

    std::vector<StateMachineRegisters> mLineTable; // Result
};

//////////////////////////////////////////////////////
// Function declerations

std::vector<StateMachineRegisters> decode_data(Header const & header);

    } // debug_line
} // dwarf