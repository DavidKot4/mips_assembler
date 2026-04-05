/**
 * @file mach_to_assm.c
 * @brief Constants used throughout codebase
 * @author Cameron Hannay
 */

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
 *@brief The definitions of the op codes, funct codes, instruction types, and the expected arguments for an instruction.
 */
extern const struct instruction_definition instruction_definitions[53];

/**
 * @brief Array of strings to represent available registers.
 */
extern const char* registers[32]; 
