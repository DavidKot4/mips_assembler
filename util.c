/**
 * @file util.c
 * @brief Utility functions
 * @author Cameron Hannay
 */

#include "include.h"

uint32_t bin_to_num(char *binary)		
{
	uint32_t num = 0;

	while (*binary != '\0') {
		num <<= 1;
		if (*binary == '1')
			num |= 1;
		binary++;
	}

	return num;
}


const uint32_t ARG_AUTO 	= 1;
const uint32_t ARG_REVERSE	= 2;

const uint32_t LINE_BUFF_SIZE = 4096;

// TODO: Add in documentation/comments since Old Jim forgot

uint32_t command_line_args(int argc, char *argv[], char** in_filename, char** out_filename)		// funct that reads args and looks for flags
{
	uint32_t result = 0;								// create variable named result and sets to 0
	for (int i = 1; i < argc; i++) {					// loop through arguments
		if (!strcmp(argv[i], "-a"))						// if current argument is -a
			result = result | ARG_AUTO;					// turns on auto flag in result
		else if (!strcmp(argv[i], "-r"))				// checks if current argument is -r
			result = result | ARG_REVERSE;				// turns on reverse flag in result
		else if (*in_filename == NULL)					// if arg is no -a or -r check input filename status
			*in_filename = argv[i];						// stores current arg as input filename
		else if (*out_filename == NULL)					// if arg is not any of the previous cases set arg to output filename
			*out_filename = argv[i];
	}
	return result;
}

void get_next_input(char* line, enum InteractiveState* int_state, uint32_t flags, char* in_filename, FILE** file)	// funct that reads next line fo input
{
	uint32_t auto_mode = (flags & ARG_AUTO) > 0;			// checks whether auto flag is turned on
	uint32_t reverse_mode = (flags & ARG_REVERSE) > 0;		// checks whether reverse flag is turned on
	
	char* str;

	if (auto_mode && in_filename != NULL) {					// if auto mode is enabled and input filename exists
		if (*file == NULL)									// if file has not been opened
			*file = fopen(in_filename, "r");				// opens input file
		str = fgets(line, LINE_BUFF_SIZE, *file);			// reads line from the file and stores result as str
		if (str == NULL && feof(*file)) {					// if reading failed 
			if (*file != NULL)								// if file pointer is valid
				fclose(*file);								// close file
			exit(0);
		}
		else if (str == NULL && ferror(*file)) {			// check if reading failed due to file error
			printf("Invalid line");							// print invalid line
			if (*file != NULL)								// if file exists
				fclose(*file);								// close file
			exit(1);
		}
		else if (*line == '\n') {							// if the line is blank
			get_next_input(line, int_state, flags, in_filename, file);	// calls funct to skip blank line
		}
		line[strcspn(line, "\n")] = 0;						
		return;
	}
	else if (auto_mode && in_filename == NULL) {			// checks if auto mode is enabled but no input file 
		str = fgets(line, LINE_BUFF_SIZE, stdin);			// reads line from input
		if (str == NULL && feof(stdin)) {					// if input ended due to end of file reach
			if (stdin != NULL)								// if stdin exists
				fclose(stdin);								// close stdin
			exit(0);
		}
		else if (str == NULL && ferror(stdin)) {			// iff reading from input caused an error
			printf("Invalid line");							// print invalid line
			if (stdin != NULL)								// if stdin exists
				fclose(stdin);								// close stdin
			exit(1);
		}
		else if (*line == '\n') {							// if input line is blank
			get_next_input(line, int_state, flags, in_filename, file);	// call funct to skip blank line
		}

		if (strchr(line, '\n') == NULL || str == NULL) {	// if there is no new line or if str is null
			printf("Invalid line"); 						// print invalid line
			if (*file != NULL)								// if file exists
				fclose(*file);								// close file
			exit(1);
		}
		line[strcspn(line, "\n")] = 0;
		return;
	}
	else if (!auto_mode) {									// if auto mode is disabled 
		while (1) {
			char choice[10];								// create array named choice
			switch (*int_state) {						 	// check current int state choose what menu prompt
				case ROOT:									// main menu state
					printf("\nPlease enter an option:\n\t(1) Assembly to Machine Code\n\t(2) Machine Code to Assembly\n\t(3) Quit\n\t(4) Corrupted Code Inspector\n");		// print menu
					printf("\n> ");
					choice[0] = 0;
					str = fgets(choice, sizeof(choice), stdin);		// read users input
					if (strchr(choice, '\n') == NULL || str == NULL) {		// if the input did not include a new line or str = null
						printf("Invalid input");							// return error
						*int_state = ROOT;									// stay at main menu state
						continue;
					}
					choice[strcspn(choice, "\n")] = 0;						// removes new line character from string choice
					if (!strcmp(choice, "1")) {								// checks whether user entered 1
						*int_state = ASM_TO_MACH;							// changes the state to assm to machine 
					}
					else if (!strcmp(choice, "2")) {						// checks whether user entered 2
						*int_state = MACH_TO_ASM;							// changes the state to machine to assm 
					}
					else if (!strcmp(choice, "3")) {						// checks whether user entered 3
						if (*file != NULL)									// checks if file is open
							fclose(*file);									// closes file
						exit(0);
					}
					else if (!strcmp(choice, "4")) {						// checks whether user entered 4
						*int_state = DEBUG;									// changes the state to debug
					}
					else {
						printf("Invalid input");							// if user did not enter 0-4
						*int_state = ROOT;									// return to main menu
					}
					break;
			
				case MACH_TO_ASM:	// case if mach to assm
					printf("\nPlease select an option:\n\t(1) Hexadecimal to Assembly\n\t(2) Binary to Assembly\n\t[3] Main Menu\n");	// mach to assm menu
					printf("\n> ");
					choice[0] = 0;											// clear choice string
					str = fgets(choice, sizeof(choice), stdin);				// read user input
					if (strchr(choice, '\n') == NULL || str == NULL) {		// check for invalid input
						printf("Invalid input");							// print invalid input
						*int_state = MACH_TO_ASM;							// set state to mach to asm
						continue;
					}
					choice[strcspn(choice, "\n")] = 0;						// remove new line from input
					if (!strcmp(choice, "1")) {								// checks if user input was 1
						*int_state = HEX_TO_ASM;							// changes state to hex to assm
					}
					else if (!strcmp(choice, "2")) {						// checks if user input was 2
						*int_state = BIN_TO_ASM;							// changes state to binary to assm
					}
					else if (!strcmp(choice, "3")) {						// checks if user input was 3
						*int_state = ROOT;									// return to main menu
					}
					else {	
						printf("Invalid input");							// if invalid input return invalid input and return to mach to assm menu
						*int_state = MACH_TO_ASM;
					}
					break;
				
				case ASM_TO_MACH:											// case if assm to machine
					printf("\n Enter a line of assembly:\n> ");				// etner assm
					str = fgets(line, LINE_BUFF_SIZE, stdin);
					if (strchr(line, '\n') == NULL || str == NULL) {		// if input is invalid
						printf("Invalid input");							// return error
						continue;
					}
					line[strcspn(line, "\n")] = 0;
					if (line[0] == 0) { 
						*int_state = ROOT;									// return to main menu
					}
					return;
				
				case HEX_TO_ASM:											// case if hex to assm
					printf("\n Enter Hex:\n> ");							// print menu
					str = fgets(line, LINE_BUFF_SIZE, stdin);				// read input
					if (strchr(line, '\n') == NULL || str == NULL) {		// checks if input is invalid
						printf("Invalid input");							// returns error
						continue;
					}
					line[strcspn(line, "\n")] = 0;							// removes new line
					if (line[0] == 0) { 									// checks for blank line
						*int_state = MACH_TO_ASM;							// return to mach to assm
					}
					return;
				
				case BIN_TO_ASM:											// case if binary to assm
					printf("\n Enter Binary:\n> ");							// print menu
					str = fgets(line, LINE_BUFF_SIZE, stdin);				// read input
					if (strchr(line, '\n') == NULL || str == NULL) {		// checks if input is invalid
						printf("Invalid input");							// return error
						continue;
					}
					line[strcspn(line, "\n")] = 0;							// remove new line
					if (line[0] == 0) { 									// check if blank line
						*int_state = MACH_TO_ASM;							// return to mach to assm
					}
					return;
				case DEBUG:													// case if debug
					printf("\n Enter Broken Binary:\n> ");					// print menu
					str = fgets(line, LINE_BUFF_SIZE, stdin);				// read input
					if (strchr(line, '\n') == NULL || str == NULL) {		// checks if input is invalid
						printf("Invalid input");							// return error
						continue;
					}
					line[strcspn(line, "\n")] = 0;							// removes new line
					if (line[0] == 0) { 									// check if blank line
						*int_state = ROOT;									// return to main menu
					}	
					return;

			}
		}
	}
	else {
		return;
	}
}
