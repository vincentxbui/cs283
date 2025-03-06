#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    command_list_t clist;

    // TODO IMPLEMENT MAIN LOOP

    // TODO IMPLEMENT parsing input to cmd_buff_t *cmd_buff

    // TODO IMPLEMENT if built-in command, execute builtin logic for exit, cd (extra credit: dragon)
    // the cd command should chdir to the provided directory; if no directory is provided, do nothing

    // TODO IMPLEMENT if not built-in command, fork/exec as an external command
    // for example, if the user input is "ls -l", you would fork/exec the command "ls" with the arg "-l"

    cmd_buff = malloc(SH_CMD_MAX);
    for (int i = 0; i < CMD_MAX; i++) {
        alloc_cmd_buff(&clist.commands[i]);
    }
    while(1){
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
        rc = build_cmd_list(cmd_buff, &clist);
        if (rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        if (exec_built_in_cmd(&clist.commands[0]) == BI_EXECUTED) {
            if (strcmp(clist.commands[0].argv[0], EXIT_CMD) == 0) {
                break;
            }
        } else {
            rc = execute_pipeline(&clist);
        }
    }
    free(cmd_buff);
    for (int i = 0; i < CMD_MAX; i++) {
        free_cmd_buff(&clist.commands[i]);
    }
    return OK;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer != NULL) {
        free(cmd_buff->_cmd_buffer);
    }
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argc = 0;
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    clear_cmd_buff(cmd_buff);
    strcpy(cmd_buff->_cmd_buffer, cmd_line);
    char *cmd_ptr = cmd_buff->_cmd_buffer;
    bool in_quotes = false;
    while (*cmd_ptr) {
        while (*cmd_ptr == SPACE_CHAR) {
            cmd_ptr++;
        }
        if (*cmd_ptr == '\0') {
            break;
        }
        if (*cmd_ptr == '"') {
            in_quotes = true;
            cmd_ptr++;
        }
        if (cmd_buff->argc < CMD_ARGV_MAX - 1) {
            cmd_buff->argv[cmd_buff->argc] = cmd_ptr;
            cmd_buff->argc++;
        } else {
            return ERR_TOO_MANY_COMMANDS;
        }
        while (*cmd_ptr && (in_quotes || *cmd_ptr != SPACE_CHAR)) {
            if (in_quotes && *cmd_ptr == '"') {
                in_quotes = false;
                *cmd_ptr = '\0';
                break;
            }
            cmd_ptr++;
        }
        if (*cmd_ptr) {
            *cmd_ptr = '\0';
            cmd_ptr++;
        }
    }
    cmd_buff->argv[cmd_buff->argc] = NULL;
    if (cmd_buff->argc == 0) {
        return WARN_NO_CMDS;
    }
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (strcmp(input, EXIT_CMD) == 0) {
        return BI_CMD_EXIT;
    }
    if (strcmp(input, "cd") == 0) {
        return BI_CMD_CD;
    }
    return BI_NOT_BI;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    switch (match_command(cmd->argv[0])) {
        case BI_CMD_EXIT:
            exit(OK_EXIT);
        case BI_CMD_CD:
            if (cmd->argc == 1) {
                return BI_EXECUTED;
            } else if (cmd->argc == 2) {
                if (chdir(cmd->argv[1]) != 0) {
                    return BI_NOT_BI;
                }
                return BI_EXECUTED;
            } else {
                return BI_NOT_BI;
            }
        default:
            return BI_NOT_BI;
    }
}

int exec_cmd(cmd_buff_t *cmd) {
    int process_id = fork();
    if (process_id < 0) {
        return ERR_EXEC_CMD;
    } else if (process_id == 0) {
        execvp(cmd->argv[0], cmd->argv);
        exit(ERR_EXEC_CMD);
    } else {
        int exit_status;
        waitpid(process_id, &exit_status, 0);
        return WEXITSTATUS(exit_status);
    }
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    int cmd_count = 0;
    char *cmd_ptr = strtok(cmd_line, PIPE_STRING);
    while (cmd_ptr != NULL && cmd_count < CMD_MAX) {
        cmd_buff_t *cmd_buff = &clist->commands[cmd_count];
        int rc = build_cmd_buff(cmd_ptr, cmd_buff);
        if (rc != OK) {
            return rc;
        }
        cmd_count++;
        cmd_ptr = strtok(NULL, PIPE_STRING);
    }
    clist->num = cmd_count;
    if (cmd_count > 0) {
        return OK;
    } else {
        return WARN_NO_CMDS;
    }
}

int execute_pipeline(command_list_t *clist) {
    int num_commands = clist->num;
    int pipes[num_commands - 1][2];
    pid_t pids[num_commands];
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0) {
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            execvp(clist->commands[i].argv[0], clist->commands[i].argv);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    } 
    return OK;
}
