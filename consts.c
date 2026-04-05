/**
 * @file consts.c
 * @brief Constants used throughout codebase.
 * @author Cameron Hannay
 */
#include "include.h"

// TODO: Add in more documentation/comments since Old Jim forgot

const uint32_t UNDEFINED;

const uint32_t MAX_5_BIT = 31;
const uint32_t MAX_6_BIT = 63;
const uint32_t MAX_16_BIT = 65535;
const uint32_t MAX_26_BIT = 67108863;

const char* registers[] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

const struct instruction_definition instruction_definitions[] = {
    // Three "Golden Standard" Instructions
    // The ADD instruction shows what each part of the instruction defintion means
    {
        "ADD",                  // MIPS instruction name
        R_TYPE,                 // Type of instruction (R, I, or J)
        "000000",               // Opcode
        "100000",               // Funct Code
        { RD, RS, RT, EMPTY }   // Registers
    },                          // This would be read as: add $rd, $rs, $rt

    {
        "ADDI",
        I_TYPE,
        "001000",
        NULL,
        { RT, RS, IMM, EMPTY }
    },

    {
        "BNE",
        I_TYPE,
        "000101",
        NULL,
        { RS, RT, IMM, EMPTY }
    },

    //TODO: Fix the bugs in the other instructions to restore functionality

    {
        "AND",
        I_TYPE,
        "001000",
        "100100",
        { RD, RS, RT, EMPTY }
    },

    {
        "ANDI",
        R_TYPE,
        "001100",
        NULL,
        { RS, RT, IMM, EMPTY }
    },

    {
        "BEQ",
        R_TYPE,
        "000000",
        "000100",
        { RS, RT, IMM, EMPTY }
    },

    {
        "DIV",
        R_TYPE,
        "000000",
        "011010",
        { RS, EMPTY, EMPTY, EMPTY }
    },

    {
        "LUI",
        I_TYPE,
        "001111",
        NULL,
        { RS, IMM, EMPTY, EMPTY }
    },

    {
        "LW",
        R_TYPE,
        "100011",
        NULL,
        { RS, IMM, RT, EMPTY }
    },

    {
        "MFHI",
        R_TYPE,
        "000000",
        "010010",
        { RS, EMPTY, EMPTY, EMPTY }
    },

    {
        "MFLO",
        R_TYPE,
        "000000",
        "010000",
        { RS, EMPTY, EMPTY, EMPTY }
    },

    {
        "OR",
        R_TYPE,
        "001101",
        "000000",
        { RD, RS, RT, EMPTY }
    },

    {
        "SLT",
        R_TYPE,
        "000000",
        "101011",
        { RD, RT, RS, EMPTY }
    },

    {
        "SUB",
        R_TYPE,
        "100010",
        "000000",
        { EMPTY, RS, RT, EMPTY }
    },

    {
        "SW",
        I_TYPE,
        "100011",
        NULL,
        { RS, IMM, RT, EMPTY }
    }
    
    // TODO: Implement MULT, ORI, and SLTI instructions once bugs are fixed
};