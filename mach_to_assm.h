/**
 * @file mach_to_assm.h
 * @brief File containing the logic to convert a machine instruction to assembly
 * @author Cameron Hannay
 */

/**
 * @brief Converts a machine instruction into assembly.
 * @param inst The provided machine instruction.
 * @param error Pointer to string to hold error message if an error occurs.
 * @return An assm_parse_result containing the instruction given when error is not set.
 * Undefined when error is set.
 */
struct assm_parse_result convert_to_assembly(uint32_t inst, char **error);
