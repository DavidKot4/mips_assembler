/**
 * @file gen_assm.h
 * @brief Code to generate assembly from a parse result
 * @author Cameron Hannay
 */

/**
 * @brief Function to generate assembly as text from a parse result.
 * @param str A buffer that will contain the generated assembly instruction if error isn't
 * set. Contents undefined if error is set.
 * @param size The size of the buffer.
 * @param parsed The assembly parse result.
 * @param error Pointer to string to hold error message if an error occurs.
 */
void generate_assembly(char *buf, size_t size, struct assm_parse_result parsed, char **error);
