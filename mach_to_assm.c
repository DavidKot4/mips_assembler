/**
 * @file mach_to_assm.c
 * @brief File containing the logic to convert a machine instruction to assembly.
 * instruction
 * @author Cameron Hannay
 */

#include "include.h"

/**
 * @brief Finds the index in instructon_definitions of the instruction corresponding to the
 * funct_code if it is R_TYPE or the op_code otherwise.
 * @param op_code The provided op_code as an integer.
 * @param funct_code The provided funct_code as an integer (nonsense if op_code isn't for
 * R_TYPE instruction).
 * @param error Pointer to string to hold error message if an error occurs.
 * @return The index of the instruction in instruction_definitions if error is not set.
 * Undefined if error is set.
 */
static size_t find_instruction_by_code(uint32_t op_code, uint32_t funct_code, char **error);


// TODO: Add in documentation/comments since Old Jim forgot

struct assm_parse_result convert_to_assembly(uint32_t inst, char **error)					// take parse and return inst name
{
	*error = NULL;																			// start with no errors

	uint32_t rd = (inst >> 11) & MAX_5_BIT;													// takes rd from instruction shifts to bits 15-11
	uint32_t rs = (inst >> 21) & MAX_5_BIT;													// takes rs from instruction shifts bits to 25-21
	uint32_t rt = (inst >> 16) & MAX_5_BIT;													// takes rt from instruciton shifts bits to 20-16
	uint32_t sa = (inst >> 6) & MAX_5_BIT;													// takes sa from instruction shifts bits to 10-6
	uint32_t imm = inst & MAX_16_BIT;														// takes imm from instruction 16 bit max
	uint32_t tar = inst & MAX_26_BIT;														// takes target from instruction 26 bit max

	uint32_t op_code = (inst >> 26) & MAX_6_BIT;											// takes  op code from first 6 bits 
	uint32_t funct_code = inst & MAX_6_BIT;													// takes funct from last 6 bits

	struct assm_parse_result assembly;														// create structure for assembly inst

	size_t ins = find_instruction_by_code(op_code, funct_code, error);						// calls function to match op code and funct code
	if (*error != NULL)																		// if function finds an error returns error
		return assembly;

	assembly.op_name = instruction_definitions[ins].op_name;								// store instruction name
	for (size_t i = 0; i < 4; i++) {														// loops through possible operand positions
		switch (instruction_definitions[ins].parts[i]) {
		case RD:																			// case when operand slot is rd register
			assembly.types[i] = REGISTER;													// label operand as register
			assembly.vals[i] = rd;															// store rd value into operand 
			break;
		case RS:																			// case when operand slot is rs register
			assembly.types[i] = REGISTER;													// label operand as register
			assembly.vals[i] = rs;															// store rs value into operand
			break;
		case RT:																			// case when operand slot is RT register
			assembly.types[i] = REGISTER;													// label operand as register
			assembly.vals[i] = rt;															// store rt value into operand
			break;
		case SA:																			// case when operand slot is SA register
			assembly.types[i] = REGISTER;													// label operand as register
			assembly.vals[i] = sa;															// store sa value into operand
			break;
		case IMM:																			// case when operand slot is immediate value																	
			assembly.types[i] = IMMEDIATE;													// labels operand as immediate
			assembly.vals[i] = imm;															// store immediate value
			break;	
		case TAR:																			// case when operand slot is a target 
			assembly.types[i] = TARGET;														// labels operand as target address
			assembly.vals[i] = tar;															// store target field		
			break;
		case EMPTY:																			// case when oeprand slot is empty
			assembly.types[i] = NONE;														// labels slot as none
			assembly.vals[i] = UNDEFINED;													// placeholder slot
			break;	
		}
	}

	return assembly;
}

static size_t find_instruction_by_code(uint32_t op_code, uint32_t funct_code, char **error)	// function that looks trhough inst definitions and returns instruction matches given opcode and funct
{
	size_t ins;
	bool found = false;																		// tracker for instruction found

	for (ins = 0; ins < sizeof(instruction_definitions)										// loops through instructions
			/ sizeof(struct instruction_definition); ins++) {								
		if (bin_to_num(instruction_definitions[ins].op_code) != op_code)					// checks current inst opcode compared to opcode mach inst
			continue;

		if (instruction_definitions[ins].type == R_TYPE) {									//checks if inst is an r type inst
			if (bin_to_num(instruction_definitions[ins].funct_code) != funct_code)			// checks if function code matches
				continue;
		}

		found = true;																		// sets found to true
		break;
	}

	if (found == false)										
		*error = "No instruction of given op code or funct code.";							// returns error if no instruction is found

	return ins;
}
