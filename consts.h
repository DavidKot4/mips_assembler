/**
 * @file consts.h
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
extern const struct instruction_definition instruction_definitions[];
extern const size_t instruction_definition_count;
/**
 * @brief Array of strings to represent available registers.
 */
extern const char* registers[32]; 

