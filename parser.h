/**
 * @file parser.h
 * @brief Parses user input of assembly and determines what kind of instruction it is to be converted into 
 * @author Cameron Hannay
 */

/**
 * @brief Uses sys_free_mem() to free all memory associated with the pcb
 * @param line String of assembly instruction to translate
 * @param error Pointer to string that will contain error message 
 * @return Instruction in binary
 */
uint32_t parse_assembly(char *line, char **error);
