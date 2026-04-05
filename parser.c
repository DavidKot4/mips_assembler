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

uint32_t parse_assembly(char *line, char **error)
{
    struct assm_parse_result result;
    *error = NULL;
    size_t length;
    char* opp_name;

    length = strcspn(line, " ");

    opp_name = (char*)malloc(sizeof(char) * (length + 1));

    if (opp_name == NULL)
    {
        *error = "Memory not allocated.";
        return UNDEFINED;
    }

    strncpy(opp_name, line, length);

    opp_name[length] = '\0';
            
    uppercase(opp_name);

    result.op_name = opp_name;

    line += length;

    char* token = strtok(line, ",");
    size_t arg;
    for (arg = 0; token != NULL; arg++)
    {
        remove_space(token);

        if (token[0] == '#')
        {
            result.types[arg] = IMMEDIATE;
            remove_pound(token);
            char* end_ptr;
            
	    union signedness_switch_trick signedness;
            signedness.i = (int16_t) strtol(token, &end_ptr, 10);
            result.vals[arg] = signedness.u;

            if(end_ptr == token || *end_ptr != '\0')
            {
                *error = "Improperly formatted immediate.";
                free(opp_name);
                opp_name = NULL;
                return UNDEFINED;
            }
        }
        else if (token[0] == '$')
        {
            result.types[arg] = REGISTER;
            result.vals[arg] = reg_lookup(token, error);
            if (*error != NULL) {
                free(opp_name);
                opp_name = NULL;
                return UNDEFINED;
            }
        }
        else
        {
            *error = "Argument isn't register or immediate (targets not yet supported).";
            free(opp_name);
            opp_name = NULL;
            return UNDEFINED; 
        }
        token = strtok(NULL, ",");
    }

    for (;arg < 4; arg++) {
        result.types[arg] = NONE;
        result.vals[arg] = UNDEFINED;
    }

    uint32_t new_val = convert_to_machine_code(result, error);
    
    free(opp_name);
    opp_name = NULL;

    return new_val;
}

static void uppercase(char *operand)
{
    size_t length = strlen(operand);
    for (size_t i = 0; i < length; i++)
    {
        operand[i] = toupper((unsigned char) operand[i]);
    }
}

static void remove_pound(char* str)
{
    int count = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != '#')
        {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

static void remove_space(char* str)
{
    int count = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] != ' ')
        {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

static uint32_t reg_lookup(char *str, char** error)
{
    for (uint32_t i = 0; i < sizeof(registers) / sizeof(char *); i++)
    {
        if (strcmp(registers[i],str) == 0)
        {
            return i;
        }
    }

    *error = "Register not found.";
    return UNDEFINED;
}
