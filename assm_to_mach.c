/**
 * @file assm_to_mach.c
 * @brief File containing the logic to convert from assembly to a machine instruction
 * @author Cameron Hannay
 */

#include "include.h"

/**
 * @brief Find the index of the instruction of the given name.
 * @param given_name The name of the op entered by the user.
 * @param error Pointer to string to hold error message if an error occurs.
 * @return Index to instruction_definitions corresponding to the given name when error isn't
 * set. Undefined when error is set.
 */
static size_t find_instruction_by_name(char *given_name, char **error);

/**
 * @brief Verify that the type of a given argument matches the type of the part of the
 * instruction it is supposed to be, and that its value does not exceed the limits for its
 * type.
 * @param part The part of the instruction the value is supposed to go to.
 * @param given_type The type of the argument.
 * @param given_val The value of the argument.
 * @param error Pointer to string to hold error message if the given argument is of the wrong
 * type or exceeds its limits.
 */
static void check_instruction_part(enum instruction_part part,
		enum assm_parse_result_type given_type, uint32_t given_val, char **error);


// TODO: Add in documentation/comments since Old Jim forgot

uint32_t convert_to_machine_code(struct assm_parse_result parsed, char **error)
{
	*error = NULL; 														// clears errors

	size_t ins = find_instruction_by_name(parsed.op_name, error);		// find instruction name (lw,add,etc) if it cannot find one returns error
	if (*error != NULL)
		return UNDEFINED;

	uint32_t result = 0; 												// create empty 32 bit inst

	result |= bin_to_num(instruction_definitions[ins].op_code) << 26;	// grab opcode from instruction and shifts to correct position

	if (instruction_definitions[ins].type == R_TYPE)					// if it is an R-type instruction add funct field
		result |= bin_to_num(instruction_definitions[ins].funct_code); 

	for (size_t i = 0; i < 4; i++) {									// process instructions
		check_instruction_part(instruction_definitions[ins].parts[i],
				parsed.types[i], parsed.vals[i], error);

		if (*error != NULL)												// return undefined if error
			return UNDEFINED;

		switch (instruction_definitions[ins].parts[i]) {				
		case RD:														// desination register
			result |= parsed.vals[i] << 11;
			break;
		case RS:														// source register
			result |= parsed.vals[i] << 21;
			break;
		case RT:														// target register
			result |= parsed.vals[i] << 16;
			break;
		case SA:														// shift amount
			result |= parsed.vals[i] << 6;
			break;
		case IMM:														// Immediate values
		case TAR:														// Target values
			result |= parsed.vals[i];
			break;
		case EMPTY:														// empty
			break;
		}
	}

	return result;														// return instruction
}

static size_t find_instruction_by_name(char *given_name, char **error)	// searches for instruction
{
	size_t ins;
	bool found = false;

	for (ins = 0; ins < sizeof(instruction_definitions)					// loops through instruction definitions and compares to strcmp, if match returns index
			/ sizeof(struct instruction_definition); ins++) {
		if (strcmp(given_name, instruction_definitions[ins].op_name) == 0) {
			found = true;
			break;
		}
	}

	if (found == false)													// if no instruction returns error
		*error = "No instruction of given name.";

	return ins;
}

static void check_instruction_part(enum instruction_part part,			// checks if the instruction is valid
		enum assm_parse_result_type given_type, uint32_t given_val, char **error)
{
	switch (part) {
	case RD:
	case RS:
	case RT:
	case SA:
		if (given_type != REGISTER) {									//checks if these cases are registers
			*error = "Missing register.";								// returns error if not register
			return;
		}
		if (given_val > MAX_5_BIT) {									// chekcs if value is 5 bits
			*error = "Invalid register.";								// returns error if it doesnt fit 5 bits
			return;
		}
		break;
	case IMM:															// checks if immediate value
		if (given_type != IMMEDIATE) {
			*error = "Missing immediate.";								// if not immediate returns error
			return;
		}
		if (given_val > MAX_16_BIT) {									// checks if immediate fits 16 bits
			*error = "Invalid immediate.";								// returns error if larger than 16 bits
			return;
		}
		break;
	case TAR:
		if (given_type != TARGET) {										// checks if value is target type
			*error = "Missing target.";									// if not target returns error
			return;
		}
		if (given_val > MAX_26_BIT) {									// checks if target is less than 26 bits
			*error = "Invalid target.";									// if larger than 26 bits returns error
			return;
		}
		break;
	case EMPTY:
		if (given_type != NONE) {										// checks if type is none				
			*error = "Too many arguments.";								// returns error if there is bits
			return;
		}
	}
}
