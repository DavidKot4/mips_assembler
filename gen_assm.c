/**
 * @file gen_assm.c
 * @brief Code to generate assembly from a parse result
 * @author Cameron Hannay

 */

#include "include.h"

// TODO: Add in documentation/comments since Old Jim forgot

void generate_assembly(char *buf, size_t size, struct assm_parse_result parsed, char **error)
{
	*error = NULL; 																			 // clear errors 

	char args[4][8];																		 // create 4 argument string
	size_t arg_count;																  		 // track argument count
	union signedness_switch_trick signedness;										   		 // convert immediate values to signed values

	for (arg_count = 0; arg_count < 4 && parsed.types[arg_count] != NONE; arg_count++) {	 // loop through operands one by one
		switch (parsed.types[arg_count]) {													 // check operand 
		case REGISTER:																		 // case if operand is a register
			strcpy(args[arg_count], registers[parsed.vals[arg_count]]);						 // copies register to correct argument slot
			break;
		case IMMEDIATE:																	     // case if operand is an immediate
			signedness.u = (uint16_t) parsed.vals[arg_count];								 // stores immediate as an unsigned 16 bit value
			sprintf(args[arg_count], "#%d", signedness.i);									 // converts immediate to a string
			break;
		case TARGET:																		 // checks if operand is a target
			*error = "Generating assembly with targets is unsupported.";					 // prints error if operand is a target
			return;
		case NONE:
			break;
		}
	}

	char *formats[5] = {																	 // create an array of strings
		"%s",																				 // opcode format
		"%s %s",																			 // opcode + operand, etc
		"%s %s, %s",
		"%s %s, %s, %s",
		"%s %s, %s, %s, %s"
	};
	int needed = snprintf(buf, size, formats[arg_count], parsed.op_name,					 // assembly string
			args[0], args[1], args[2], args[3]);

	if (needed >= size)																		 // report error if buffer too small
		*error = "Buffer too small for assembly instruction.";
}
