/**
 * @file assm_to_mach.h
 * @brief File containing the logic to convert from assembly to a machine instruction
 * @author Cameron Hannay
 */

/**
 * @brief Converts an assembly instruction into a machine instruction.
 * @param parsed A struct containing the assembly instruction to convert.
 * @param error Pointer to string to hold error message if an error occurs.
 * @return A uint32_t containing the machine instruction when error is not set. Undefined when
 * error is set.
 */
uint32_t convert_to_machine_code(struct assm_parse_result parsed, char **error);
