/**
 * @file types.h
 * @brief Struct and enum declarations
 * @author Cameron Hannay
 */

/**
 * @brief The types of arguments in assembly.
 */
enum assm_parse_result_type {
	REGISTER,
	IMMEDIATE,
	TARGET,
	NONE
};

/** 
 * @brief A structure for holding an assembly instruction. Note: op_name is expected to be in all caps. 
 * The type is expected to be NONE when there are not enough arguments to fill the array.
 * The number in vals is undefined when the corresponding type is NONE.
 */
struct assm_parse_result {
	char *op_name;
	enum assm_parse_result_type types[4];
	uint32_t vals[4];
};

/**
 * @brief The types of instructions.
 */
enum instruction_type {
	R_TYPE,
	I_TYPE,
	J_TYPE
};

/**
 * @brief The parts of a machine instruction.
 */
enum instruction_part {
	RD,
	RS,
	RT,
	SA,
	IMM,
	TAR,
	EMPTY
};

/**
 * @brief A structure to hold the defintion of an instruction. Note: op_name is expected to be all caps. 
 * funct_code should be NULL when type isn't R_TYPE. 
 * The parts represent the mapping of arguments to their function type. 
 * The remaining values in parts should be EMPTY for instructions with less than four operands.
 */
struct instruction_definition {
	char *op_name;
	enum instruction_type type;
	char *op_code;
	char *funct_code;
	enum instruction_part parts[4];
};


/**
 * @brief A union for changing the type of a uint16_t to int16_t or vice versa
 * without altering the bits. Useful for marshalling within project.
 */
union signedness_switch_trick {
	uint16_t u;
	int16_t i;
};

/**
 * @brief Enum for the state of the interactive assembler mode
 */
enum InteractiveState {
	ROOT,

	ASM_TO_MACH,

	MACH_TO_ASM,
	HEX_TO_ASM,
	BIN_TO_ASM,

	DEBUG,

	INACTIVE
};
