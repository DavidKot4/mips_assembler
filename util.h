/**
 * @file util.h
 * @brief Utility functions
 * @author Cameron Hannay
 */

/**
 * @brief Converts a string containing an unsigned binary number into an integer.
 * @param binary A string containing an unsigned binary number that can be represented with 32 bits.
 * @return The binary number as an integer.
 */
uint32_t bin_to_num(char *binary);

/**
 * @brief Set when the user has indicate the automatic mode
 */
extern const uint32_t ARG_AUTO;
extern const uint32_t ARG_REVERSE;
extern const uint32_t LINE_BUFF_SIZE;

/**
 * @brief Handle command line args
 * @param argc Pass through from main
 * @param argv Pass through from main
 * @return uint32_t The bit flags for the returned options
 */
uint32_t command_line_args(int argc, char *argv[], char** in_filename, char** out_filename);

/**
 * @brief Get the next input line
 * @param flags The flags from the command line arguments
 * @param in_filename Input filename from command line arguments
 * @param out_filename Output filename from command line arguments
 * @return char* The pointer to the next line in the input
 */
void get_next_input(char* line, enum InteractiveState* int_state, uint32_t flags, char* in_filename, FILE** file);
