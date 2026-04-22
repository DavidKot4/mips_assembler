/**
 * @file parser.c
 * @brief Parses user input of assembly and determines what kind of instruction it is to be converted into
 * @author Cameron Hannay
 */

#include "include.h"

/**
 * @brief Takes the string and turns each character to uppercase
 * @param operand String to convert to uppercase
 */
static void uppercase(char *operand);

/**
 * @brief Takes the string compares it with each register type to find a match
 * @param operand String to lookup
 */
static uint32_t reg_lookup(char *str, char** error);

/**
 * @brief Removes whitespace from a string
 * @param str String to remove whitespace from
 */
static void remove_space(char* str);

/**
 * @brief Removes pound symbol from string
 * @param str String to remove pound symbol from
 */
static void remove_pound(char* str);

// TODO: Add in documentation/comments since Old Jim forgot

uint32_t parse_assembly(char *line, char **error)						// main function returns machine code 
{
    struct assm_parse_result result;									// stuct stores results 
    *error = NULL;														// clear errors
    size_t length;														// variable
    char* opp_name;														// pointer to character array

    length = strcspn(line, " ");										// sets length to how many characters before the first space

    opp_name = (char*)malloc(sizeof(char) * (length + 1));				// allocating memory for opcode string

    if (opp_name == NULL)												// if previous line failed
    {
        *error = "Memory not allocated.";								// sends error
        return UNDEFINED;
    }

    strncpy(opp_name, line, length);									// copies op funct

    opp_name[length] = '\0';											//create C string
            
    uppercase(opp_name);												// converts opcode to uppercase

    result.op_name = opp_name;											// stores opcode string into result

    line += length;														// moves line pointer forward by length

    char* token = strtok(line, ",");									// splits the tokens by commas
    size_t arg;															// create variable arg
    for (arg = 0; token != NULL; arg++)									// loop for as many tokens
    {
        remove_space(token);

        if (token[0] == '#')											// if token starts with #
        {
            result.types[arg] = IMMEDIATE;								// treat as immediate				
            remove_pound(token);										// remove #
            char* end_ptr;												// end pointer
            
	    union signedness_switch_trick signedness;						// create variable named signedness
            signedness.i = (int16_t) strtol(token, &end_ptr, 10);		// convert token string into integer
            result.vals[arg] = signedness.u;							// stores bits as unsigned value 

            if(end_ptr == token || *end_ptr != '\0')					// checks if any previous lines failed
            {
                *error = "Improperly formatted immediate.";
                free(opp_name);
                opp_name = NULL;
                return UNDEFINED;
            }
        }
        else if (token[0] == '$')										// checks if token is a register
        {
            result.types[arg] = REGISTER;								// markes token as register
            result.vals[arg] = reg_lookup(token, error);				// look up register values stores that value
            if (*error != NULL) {										// checks if previous line failed
                free(opp_name);											// free memory 
                opp_name = NULL;										// set opp_name pointer to null
                return UNDEFINED;
            }
        }
        else
        {
            *error = "Argument isn't register or immediate (targets not yet supported).";	// if not previous cases, return error needs to be register or immediate input
            free(opp_name);												// free memory
            opp_name = NULL;											// set opp_name pointer to null
            return UNDEFINED; 
        }
        token = strtok(NULL, ",");										// get the next token from the string
    }

    for (;arg < 4; arg++) {												// starts loop to fill unused arguments
        result.types[arg] = NONE;										// marks unused args as none
        result.vals[arg] = UNDEFINED;									// marks unused values as undefined
    }

    uint32_t new_val = convert_to_machine_code(result, error);			// call funct to convert parsed result into 32 bit machine code
    
    free(opp_name);														// free memory
    opp_name = NULL;

    return new_val;
}

static void uppercase(char *operand)									// uppercase function
{
    size_t length = strlen(operand);									// grab lengtrh of string
    for (size_t i = 0; i < length; i++)									// loop through every character in the string
    {
        operand[i] = toupper((unsigned char) operand[i]);				// convert each character to uppercase
    }
}

static void remove_pound(char* str)										// funct to remove # from string
{
    int count = 0;														// set count pointer to 0

    for (int i = 0; i < strlen(str); i++)								// loop through every character in string
    {
        if (str[i] != '#')												// check if current character is not pound
        {
            str[count++] = str[i];										// increase count
        }
    }
    str[count] = '\0';									
}

static void remove_space(char* str)										// funct to remove spaces from string
{
    int count = 0;														// set count to 0

    for (int i = 0; i < strlen(str); i++)								// loops through all characters in the string
    {
        if (str[i] != ' ')												// checks if character is a space
        {
            str[count++] = str[i];										// increases count
        }
    }
    str[count] = '\0';
}

static uint32_t reg_lookup(char *str, char** error)						// funct that finds reg name and returns a number
{
    for (uint32_t i = 0; i < sizeof(registers) / sizeof(char *); i++)	// loops through registers array
    {
        if (strcmp(registers[i],str) == 0)								// compare current register with input register
        {
            return i;													// return register number
        }
    }

    *error = "Register not found.";										// if not found returns error message
    return UNDEFINED;
}
