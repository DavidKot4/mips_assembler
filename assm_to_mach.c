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
	*error = NULL;

	size_t ins = find_instruction_by_name(parsed.op_name, error);
	if (*error != NULL)
		return UNDEFINED;

	uint32_t result = 0;

	result |= bin_to_num(instruction_definitions[ins].op_code) << 26;

	if (instruction_definitions[ins].type == R_TYPE)
		result |= bin_to_num(instruction_definitions[ins].funct_code);

	for (size_t i = 0; i < 4; i++) {
		check_instruction_part(instruction_definitions[ins].parts[i],
				parsed.types[i], parsed.vals[i], error);

		if (*error != NULL)
			return UNDEFINED;

		switch (instruction_definitions[ins].parts[i]) {
		case RD:
			result |= parsed.vals[i] << 11;
			break;
		case RS:
			result |= parsed.vals[i] << 21;
			break;
		case RT:
			result |= parsed.vals[i] << 16;
			break;
		case SA:
			result |= parsed.vals[i] << 6;
			break;
		case IMM:
		case TAR:
			result |= parsed.vals[i];
			break;
		case EMPTY:
			break;
		}
	}

	return result;
}

static size_t find_instruction_by_name(char *given_name, char **error)
{
	size_t ins;
	bool found = false;

	for (ins = 0; ins < sizeof(instruction_definitions)
			/ sizeof(struct instruction_definition); ins++) {
		if (strcmp(given_name, instruction_definitions[ins].op_name) == 0) {
			found = true;
			break;
		}
	}

	if (found == false)
		*error = "No instruction of given name.";

	return ins;
}

static void check_instruction_part(enum instruction_part part,
		enum assm_parse_result_type given_type, uint32_t given_val, char **error)
{
	switch (part) {
	case RD:
	case RS:
	case RT:
	case SA:
		if (given_type != REGISTER) {
			*error = "Missing register.";
			return;
		}
		if (given_val > MAX_5_BIT) {
			*error = "Invalid register.";
			return;
		}
		break;
	case IMM:
		if (given_type != IMMEDIATE) {
			*error = "Missing immediate.";
			return;
		}
		if (given_val > MAX_16_BIT) {
			*error = "Invalid immediate.";
			return;
		}
		break;
	case TAR:
		if (given_type != TARGET) {
			*error = "Missing target.";
			return;
		}
		if (given_val > MAX_26_BIT) {
			*error = "Invalid target.";
			return;
		}
		break;
	case EMPTY:
		if (given_type != NONE) {
			*error = "Too many arguments.";
			return;
		}
	}
}
