#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const uint8_t N_TESTS = 7;
const char tests[7][41] = {
    "test_file_copn_ok.txt",
    "test_file_csq_not_ok_1.txt",
    "test_file_csq_not_ok_2.txt",
    "test_file_csq_not_ok_3.txt",
    "test_file_csq_not_ok_4_no_cr_present.txt",
    "test_file_csq_ok.txt",
    "test_file_simple_at_ok.txt"};
void read_parse_file(const char *filename)
{
    FILE *fp;
    char buff[AT_COMMAND_MAX_LINE_SIZE];
    printf("Opening file: %s\n", filename);
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error opening the file \n");
        exit(-1);
    }
    uint8_t finalState;
    while (fgets(buff, AT_COMMAND_MAX_LINE_SIZE, fp) != NULL)
    {
        finalState = at_parse_line(buff);
        if (date.status == FSM_READY_OK)
        {
            printf("FSM READY OK\n");
            printf("Data is");
            printData();
            printf("Lines: %d\n\n", date.line_count);
            date.status = FSM_RESET;

            commands[commands_length++] = date;
        }
        else if (date.status == FSM_READY_WITH_ERROR)
        {
            printf("FSM READY WITH ERROR\n");
            printf("State where the error occured: %d\n\n", finalState);
            date.status = FSM_RESET;
        }
    }
    printf("\n");
    printf("S-au citit %d comenzi ( cu statusul \033[0;32mFSM_READY_OK\033[0m ) \n", commands_length);
}
int main(int argc, char *argv[])
{
    char *file = argv[1];
    if (!file)
    {
        printf("File parameter was not provided \n");
        exit(EXIT_FAILURE);
    }
    read_parse_file(file);
    // for (int i = 0; i < N_TESTS; i++)
    // {
    //     read_parse_file(tests[i]);
    // }
    // return 0;
}