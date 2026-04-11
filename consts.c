/**
 * @file consts.c
 * @brief Constants used throughout codebase.
 * @author Cameron Hannay
 */
#include "include.h"
// TODO: Add in more documentation/comments since Old Jim forgot

const uint32_t UNDEFINED;


/*
 * These constants represent the maximum unsigned values that can be stored
 * in common MIPS instruction field sizes.
 *
 * They are mainly used when checking whether a parsed value can fit inside
 * a given field before encoding the instruction into machine code.
 *
 * 5-bit  field max = 2^5  - 1 = 31
 * 6-bit  field max = 2^6  - 1 = 63
 * 16-bit field max = 2^16 - 1 = 65535
 * 26-bit field max = 2^26 - 1 = 67108863
 */
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
    // Three "Golden Standard" Instructions (ADD, ADDI, and BNE)
    // These were provided as known examples in the starter code.
    // They were used as templates when repairing the other instruction definitions and when adding new instructions.
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
        NULL,                   //I-Type instructions do not have a funct code, so we set this to NULL
        { RT, RS, IMM, EMPTY }  // addi $rt, $rs, immediate
    },

    {
        "BNE",
        I_TYPE,
        "000101",
        NULL,                   //I-Type instructions do not have a funct code
        { RS, RT, IMM, EMPTY }  // bne $rs, $rt, offset
    },

    // Fixed existing instructions:
    // The entries below originally contained bugs in type, opcode, funct, and/or operand ordering. 
    // These were corrected to match real MIPS encodings and expected assembly syntax.
    {
        "AND",
        R_TYPE,                 // FIXED: was incorrectly marked as I_TYPE
        "000000",               // FIXED: R-type AND uses opcode 000000
        "100100",               // FIXED: correct funct for AND
        { RD, RS, RT, EMPTY }   // and $rd, $rs, $rt
    },

    {
        "ANDI",
        I_TYPE,                 // FIXED: was incorrectly marked as R_TYPE
        "001100",               // Correct opcode for andi
        NULL,                   // FIXED: I-type instructions do not use funct
        { RT, RS, IMM, EMPTY }  // FIXED: correct operand order
    },

    {
        "BEQ",
        I_TYPE,                 // FIXED: was incorrectly marked as R_TYPE
        "000100",               // FIXED: correct opcode for beq
        NULL,                   // FIXED: BEQ is I-type, so no funct field
        { RS, RT, IMM, EMPTY }  // beq $rs, $rt, offset
    },

    {
        "DIV",
        R_TYPE,
        "000000",               // Correct R-type opcode
        "011010",               // Correct funct for div
        { RS, RT, EMPTY, EMPTY }/* FIXED: division uses rs and rt only; result goes to HI/LO, not rd */
    },

    {
        "LUI",
        I_TYPE,
        "001111",
        NULL,
        { RT, IMM, EMPTY, EMPTY } /* FIXED: operand order is: lui $rt, imm, not rs, imm */
    },

    {
        "LW",
        I_TYPE,                 // FIXED: was incorrectly marked as R_TYPE
        "100011",               // Correct opcode for lw
        NULL,
        { RT, IMM, RS, EMPTY }  /* FIXED: project parser expects operands in
                                   the order rt, imm, rs
                                   corresponding to lw $rt, offset($rs) */
    },

    {
        "MFHI",
        R_TYPE,
        "000000",
        "010000",               // FIXED: correct funct for mfhi
        { RD, EMPTY, EMPTY, EMPTY } /* FIXED: mfhi writes to rd only */
    },

    {
        "MFLO",
        R_TYPE,
        "000000",
        "010010",               // FIXED: correct funct for mflo
        { RD, EMPTY, EMPTY, EMPTY } /* FIXED: mflo writes to rd only */
    },

    {
        "OR",
        R_TYPE,
        "000000",               // FIXED: OR is R-type, not immediate OR
        "100101",               // FIXED: correct funct for OR
        { RD, RS, RT, EMPTY }   // or $rd, $rs, $rt
    },

    {
        "SLT",
        R_TYPE,
        "000000",
        "101010",               // FIXED: correct funct for slt
        { RD, RS, RT, EMPTY }   // FIXED: corrected operand order
    },

    {
        "SUB",
        R_TYPE,
        "000000",               // FIXED: opcode was incorrectly set to funct
        "100010",               // FIXED: correct funct for sub
        { RD, RS, RT, EMPTY }   // FIXED: sub writes result into rd
    },

    {
        "SW",
        I_TYPE,
        "101011",               // FIXED: correct opcode for sw
        NULL,
        { RT, IMM, RS, EMPTY }  /* FIXED: project parser expects operands in
                                   the order rt, imm, rs
                                   corresponding to sw $rt, offset($rs) */
    },

    // Added Required New Instructions
    // These were not present in the broken starter version and were added as part of the project requirements.
    {
        "MULT",
        R_TYPE,
        "000000",
        "011000",               // funct code for mult
        { RS, RT, EMPTY, EMPTY }/* mult uses rs and rt only;
                                   result is stored in HI/LO */
    },

    {
        "ORI",
        I_TYPE,
        "001101",               // opcode for ori
        NULL,
        { RT, RS, IMM, EMPTY }  // ori $rt, $rs, imm
    },

    {
        "SLTI",
        I_TYPE,
        "001010",               // opcode for slti
        NULL,
        { RT, RS, IMM, EMPTY }  // slti $rt, $rs, imm
    }
};

/*
 * The instruction count is exposed separately so other source files can
 * iterate through the table safely without hard-coding a size.
 */
const size_t instruction_definition_count =
    sizeof(instruction_definitions) / sizeof(instruction_definitions[0]);
