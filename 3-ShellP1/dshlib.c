#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    memset(clist, 0, sizeof(command_list_t));
    clist->num = 0;
    char *command = strtok(cmd_line, PIPE_STRING);

    while (command) {
        while (*command == SPACE_CHAR) {
            command++;
        }
        int i = strlen(command) - 1;
        while (i >= 0 && command[i] == SPACE_CHAR) {
            command[i] = '\0'; 
            i--;
        }
        if (strlen(command) == 0) {
            command = strtok(NULL, PIPE_STRING);
            continue;
        }
        if (clist->num >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }
        char *start_of_command = strchr(command, SPACE_CHAR);
        if (start_of_command) {
            *start_of_command = '\0';
            start_of_command++;
            while (*start_of_command == SPACE_CHAR) {
                start_of_command++;
            }
        }
        if (strlen(command) >= EXE_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        if (start_of_command && strlen(start_of_command) >= ARG_MAX) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        strcpy(clist->commands[clist->num].exe, command);
        if (start_of_command) {
            strcpy(clist->commands[clist->num].args, start_of_command);
        } else {
            clist->commands[clist->num].args[0] = '\0';
        }
        clist->num++;
        command = strtok(NULL, PIPE_STRING);
    }
    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }

    return OK;
}
