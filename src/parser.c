#include "parser.h"
#include <stdio.h>
#include <string.h>
int commands_length = 0;

FSM_RETURN_VALUE at_command_parse(const uint8_t current_chr, uint8_t *finalState)
{
    static uint8_t state = 0;
    static uint8_t index_col = 0;

    *finalState = state;
#ifdef DEBUG
    printf("State %d\n", state);
    printf("parsing %d (%c)\n", current_chr, (char)current_chr);
#endif

    if (date.status == FSM_RESET)
    {
        for (int i = 0; i < date.line_count; i++)
        {
            for (int j = 0; j < AT_COMMAND_MAX_LINE_SIZE; j++)
            {
                date.data[i][j] = -1;
            }
        }
        // reset the flag
        date.flag = NO_FLAG;
        state = 0;
        date.line_count = 0;
        index_col = 0;
        date.status = FSM_NOT_READY;
    }

    if (current_chr == LF)
    {

        if (date.line_count < AT_COMMAND_MAX_LINES)
        {
            date.data[date.line_count][index_col] = '\0';
            index_col = 0;
            date.line_count++;
        }
    }
    switch (state)
    {
    case 0:
        if (current_chr == CR) // sau 13, check ascii table
        {
            state = 1;
            //init;
            date.line_count = 0;
            date.status = FSM_NOT_READY;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 1:
        if (current_chr == LF)
        {
            state = 2;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 2:
        if (current_chr == 'O')
        {
            state = 10;
            return FSM_NOT_READY;
        }
        else if (current_chr == 'E')
        {
            state = 3;
            return FSM_NOT_READY;
        }
        else if (current_chr == '+')
        {
            state = 14;
            return FSM_NOT_READY;
        }
        else
        {
            state = 14;
            date.flag = SPECIAL_TRANSMISSION;
            return FSM_NOT_READY;
            // return FSM_INVALID;
        }
    case 3:
        if (current_chr == 'R')
        {
            state = 4;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 4:
        if (current_chr == 'R')
        {
            state = 5;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 5:
        if (current_chr == 'O')
        {
            state = 6;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 6:
        if (current_chr == 'R')
        {
            state = 7;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 7:
        if (current_chr == CR)
        {
            state = 8;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 8:
        if (current_chr == LF)
        {
            state = 0;
            return FSM_READY_OK;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 10:
        if (current_chr == 'K')
        {
            state = 11;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 11:
        if (current_chr == CR)
        {
            state = 12;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 12:
        if (current_chr == LF)
        {
            state = 0;
            return FSM_READY_OK;
        }
        return FSM_READY_WITH_ERROR;
    case 14:
    {
        if (32 <= current_chr && current_chr <= 126)
        {
            if (date.line_count < AT_COMMAND_MAX_LINES)
            {
                date.data[date.line_count][index_col++] = current_chr;
            }
            return FSM_NOT_READY;
        }
        else if (current_chr == CR)
        {
            state = 15;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    }
    case 15:
        if (current_chr == LF)
        {
            state = 16;
        }
        return FSM_NOT_READY;
    case 16:
        if (current_chr == '+')
        {
            state = 14;
        }
        else if (current_chr == CR)
        {
            state = 17;
        }
        return FSM_NOT_READY;
    case 17:
        if (current_chr == LF)
        {
            state = 18;
            return FSM_NOT_READY;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
    case 18:
        if (current_chr == 'O')
        {
            state = 10;
        }
        else if (current_chr == 'E')
        {
            state = 3;
        }
        else
        {
            return FSM_READY_WITH_ERROR;
        }
        return FSM_NOT_READY;
    default:
        printf("Default for value %d \n", current_chr);
        break;
    }
    return FSM_READY_WITH_ERROR;
}
void printData()
{
    for (int i = 0; i < date.line_count; i++)
    {
        for (int j = 0; date.data[i][j] != '\0'; j++)
        {
            if (date.data[i][j] != 0)
            {
                printf("%c", date.data[i][j]);
            }
        }
        printf("\n");
    }
    if (date.flag != NO_FLAG)
    {
        printf("Has flag  %d\n", date.flag);
    }
}

uint8_t at_parse_line(const char *line)
{

    uint8_t finalState;
    const uint8_t condition = date.status != FSM_READY_OK && date.status != FSM_READY_WITH_ERROR;

    if (condition)
    {

#ifdef DEBUG
        printf("Line is %s\n", line);
        printf("Condition is %d , status is %d\n", condition, date.status);
#endif
        for (uint8_t i = 0; i < strlen(line) && (date.status == FSM_NOT_READY || date.status != FSM_READY_WITH_ERROR); i++)
        {
            uint8_t status = at_command_parse(line[i], &finalState);
            date.status = status;
            if (status != FSM_NOT_READY)
            {
                return finalState;
            }
        }
    }
    return finalState;
}