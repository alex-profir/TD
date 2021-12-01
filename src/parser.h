#include <stdint.h>

#ifndef __AT_COMMAND_PARSER__
#define __AT_COMMAND_PARSER__

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 100
#define AT_MAX_COMMANDS 100
#define AT_COMMAND_FLAG_SIZE 10

// Flag enum
typedef enum
{
    NO_FLAG = 0,
    SPECIAL_TRANSMISSION
} AT_COMMAND_FLAG;

// Data types
typedef struct
{
    uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE];
    uint8_t line_count;
    uint8_t status;
    AT_COMMAND_FLAG flag;
} AT_COMMAND_DATA;

AT_COMMAND_DATA commands[AT_MAX_COMMANDS];
AT_COMMAND_DATA date;

int commands_length;

// State Machine States
typedef enum
{
    FSM_NOT_READY,
    FSM_READY_OK,
    FSM_READY_WITH_ERROR,
    FSM_INVALID,
    FSM_RESET
} FSM_RETURN_VALUE;

// taken from the ascii table
enum
{
    CR = 13,
    LF = 10
};

// decls

FSM_RETURN_VALUE at_command_parse(const uint8_t current_chr, uint8_t *finalState);
// returning the final state the FSM was in
uint8_t at_parse_line(const char *line);
void printData();

#endif