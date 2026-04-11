/**
 * @file consts.h
 * @brief Constants used throughout codebase
 * @author Cameron Hannay
 */

#ifndef CONSTS_H
#define CONSTS_H

#include "include.h"
/**
 *@brief An integer to be used when a value is supposed to be undefined.
 */
extern const uint32_t UNDEFINED;

/**
 * @brief Maximum values for several n-bit integers for use with bit fiddling.
 */
extern const uint32_t MAX_5_BIT;
extern const uint32_t MAX_6_BIT;
extern const uint32_t MAX_16_BIT;
extern const uint32_t MAX_26_BIT;


/**
 * @brief Array of strings to represent available registers.
 */
extern const char* registers[32]; 

/**
 * @brief Main instruction definition table for supported MIPS instructions.
 *
 * Each entry contains the name, instruction type, opcode, optional funct field, and expected assembly argument layout.
 *
 * Common argument symbols used in the table:
 *   RD    = destination register
 *   RS    = first source register
 *   RT    = second source register / target register
 *   IMM   = immediate value
 *   EMPTY = unused argument position
 *
 * This table is used by both assembly-to-machine and machine-to-assembly
 * translation logic.
 */
extern const struct instruction_definition instruction_definitions[];

/**
 * @brief Number of valid entries currently stored in instruction_definitions.
 *
 * This was added so other files can iterate safely through the instruction
 * table without relying on a hard-coded size.
 */
extern const size_t instruction_definition_count;

#endif
