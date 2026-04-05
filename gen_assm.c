/**
 * @file gen_assm.c
 * @brief Code to generate assembly from a parse result
 * @author Cameron Hannay

 */

#include "include.h"

// TODO: Add in documentation/comments since Old Jim forgot

void generate_assembly(char *buf, size_t size, struct assm_parse_result parsed, char **error)
{
	*error = NULL;

	char args[4][8]; 
	size_t arg_count;
	union signedness_switch_trick signedness;

	for (arg_count = 0; arg_count < 4 && parsed.types[arg_count] != NONE; arg_count++) {
		switch (parsed.types[arg_count]) {
		case REGISTER:
			strcpy(args[arg_count], registers[parsed.vals[arg_count]]);
			break;
		case IMMEDIATE:
			signedness.u = (uint16_t) parsed.vals[arg_count];
			sprintf(args[arg_count], "#%d", signedness.i);
			break;
		case TARGET:
			*error = "Generating assembly with targets is unsupported.";
			return;
		case NONE:
			break;
		}
	}

	char *formats[5] = {
		"%s",
		"%s %s",
		"%s %s, %s",
		"%s %s, %s, %s",
		"%s %s, %s, %s, %s"
	};
	int needed = snprintf(buf, size, formats[arg_count], parsed.op_name,
			args[0], args[1], args[2], args[3]);

	if (needed >= size)
		*error = "Buffer too small for assembly instruction.";
}
