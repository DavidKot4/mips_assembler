/**
 * @file main.c
 * @brief Used to scan user input and run the assembler/disassembler tool
 * @author Cameron Hannay
 */

#include "include.h"

// TODO: Add in documentation/comments since Old Jim forgot

int main(int argc, char *argv[])  										// create main function
{
	// Call the command line argument parser
	char* in_filename = NULL;											// stores input file 								
	char* out_filename = NULL;											// stores output file
	uint32_t flags;														// stores flags
	flags = command_line_args(argc, argv, &in_filename, &out_filename);	// sets filenames

	enum InteractiveState int_state;									// tracks current state of program

	if (flags & ARG_AUTO) {												// if auto mode flag goes off
		int_state = INACTIVE;											// disable interactive state
	}
	else {
		printf("Welcome to the MIPS-Translatron 3000 Tool\n");			// welcome message
		printf("ByteForge Systems ©2012\n");							// System message
		int_state = ROOT;											
	}
	
	// Create empty file pointer for reading in
	FILE* in_file = NULL;												// file pointer

	while (1) {															// infinite loop
		char line[LINE_BUFF_SIZE];										// buffer
		get_next_input(line, &int_state, flags, in_filename, &in_file);	// gets next input line updates program state
		char* error = NULL;												// error message
		char* end;														// track where parsing ends
		struct assm_parse_result parse_result;							// stores decoded assembly inst
		uint32_t inst_in;												// stores binary inst input at int
		uint32_t result;												// stores result of assembly conversion
		char decompile_result[LINE_BUFF_SIZE];							
		switch (int_state) {											// choose action based on need
			case ASM_TO_MACH:											// assembly to machine code
				// Call conversion from assembly to binary
				result = parse_assembly(line, &error);					// converts assembly into 32 bit machine code
				// Check for errors in parsing and assembling
				if (error != NULL) {									// if parsing fails gives error
					printf("%s\n", error);
					continue;
				}
				// Print the result in binary and hexadecimal
				printf("Hex: 0x%08X Binary:", result);					// prints result
				for (int i = 0; i < 32; i++) {							// loop through the 32 bits
					if (i % 4 == 0 && i != 0)							
						printf(" ");									// print space
					printf("%d", result / (1 << 31));					// print left most bit
					result *= 2;
				}
				printf("\n");											
				break;
			case HEX_TO_ASM:											// hexadecimal to assembly
				// First convert string hexadecimal to string binary
				inst_in = strtoul(line, &end, 16);						// convert hex string input into unsigned int
				// Call binary to assembly conversion and print out
				parse_result = convert_to_assembly(inst_in, &error);	// decpde machine code 
				if (error != NULL) {									// print error if failed
					printf("%s\n", error);
					continue;
				}
				generate_assembly(decompile_result, LINE_BUFF_SIZE, parse_result, &error);	//turn inst structure into assembly
				if (error != NULL) {									// print error if error
					printf("%s\n", error);
					continue;
				}
				printf("%s\n\n", decompile_result);			
				break;
			case BIN_TO_ASM:											// binary to assembly 						
				// First get the binary input as uint32_t	
				inst_in = strtoul(line, &end, 2);						// convert binrary to unsigned int
				// Call binary to assembly conversion and print out
				parse_result = convert_to_assembly(inst_in, &error);	// decode binary inst into assembly structure
				if (error != NULL) {									// if error print error
					printf("%s\n", error);
					continue;
				}
				generate_assembly(decompile_result, LINE_BUFF_SIZE, parse_result, &error);	//convert parsed inst into assembly
				if (error != NULL) {									// if error print error
					printf("%s\n", error);
					continue;
				}
				printf("%s\n\n", decompile_result);						// print results
				break;
			case DEBUG:													// debug 
				// First get the binary input as uint32_t
				inst_in = strtoul(line, &end, 2);

				/* -------------------------------------------------------------------------
				* BONUS CHALLENGE: Automated Corrupted Code Inspector
				* -------------------------------------------------------------------------
				* Currently, this feature only acknowledges the input. 
				* To earn extra credit, implement a loop that:
				* 1. Flips each of the 32 bits of 'inst_in' one by one using an XOR mask.
				* 2. Attempts to disassemble each flipped version.
				* 3. Prints out valid MIPS instructions as suggestions.
				* ------------------------------------------------------------------------- */
				
				printf("Broken Binary Received: %s\n", line);			
				printf("Bonus Logic Not Yet Implemented. Returning to Root...\n");
				int_state = ROOT;
				
				break;

			case INACTIVE:
				if (flags & ARG_REVERSE) {
					// First convert string hexadecimal to string binary
					inst_in = strtoul(line, &end, 2);
					// Call conversion from binary to assembly
					parse_result = convert_to_assembly(inst_in, &error);
					if (error != NULL) {
						printf("%s\n", error);
						continue;
					}
					generate_assembly(decompile_result, LINE_BUFF_SIZE, parse_result, &error);
					if (error != NULL) {
						printf("%s\n", error);
						continue;
					}
					printf("%s\n", decompile_result);
				}
				else {
					// Call conversion from assembly to binary
					result = parse_assembly(line, &error);
					// Check for errors in parsing and assembling
					if (error != NULL) {
						printf("%s\n", error);
						continue;
					}
					// Print the result in binary
					for (int i = 0; i < 32; i++) {
						printf("%d", result / (1 << 31));
						result *= 2;
					}
					printf("\n");
				}
				break;
		}
	}
}
