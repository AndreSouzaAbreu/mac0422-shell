#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

/******************************************************************************/
/* MACROS */

#define NOT !
#define CMD_COUNT 4

/******************************************************************************/
/* TYPE DECLARATIONS */

typedef int (*command_t)(char*);

/******************************************************************************/
/* DECLARATION OF INTERNAL FUNCTIONS */

/* commands */
int protege_pra_caramba(char* filepath);
int libera_geral(char* filepath);
int rode_veja(char* filepath);
int rode(char* filepath);

/* helper functions */
int cmd_exists(char* cmd);
int get_cmd_index(char* cmd);
int call_cmd(char* cmd, char* arg);
int read_cmd(char* cmd, char* arg);
void print_prompt(void);

/******************************************************************************/
/* CONSTANTS */

static const char* CMD_NAME_LIST[CMD_COUNT] = {
    "protegepracaramba",
    "liberageral",
    "rodeveja",
    "rode"
};
static const command_t CMD_LIST[CMD_COUNT] = {
    protege_pra_caramba,
    libera_geral,
    rode_veja,
    rode
};

/******************************************************************************/
/* MAIN PROGRAM */

/**
 * Simpified shell for Operating Systems class assigment.
 * It reads commands from standard input and executes it.
 * Every supported command is of the form: <command> <filepath>.
 * */
int main(int argc, char** argv)
{
    char cmd[30], arg[256];

    while (read_cmd(cmd, arg) == 2) {
        /* validate command */
        if (NOT cmd_exists(cmd)) {
            printf("ERROR: invalid command %s\n", cmd);
            continue;
        }
        /* execute command */
        if (call_cmd(cmd, arg) != 0) {
            printf("ERROR: could not run command\n");
            continue;
        }
    }

    return 0;
}

/******************************************************************************/
/* ╭──────────────────────────────────────────────────────────╮
 * │           IMPLEMENTATION OF HELPER FUNCTIONS             │
 * ╰──────────────────────────────────────────────────────────╯
 */

/* Check if the given command is valid */
int cmd_exists(char* cmd) {
    return (get_cmd_index(cmd) != -1);
}

/* Get the index of the given command in the command list */
int get_cmd_index(char* cmd)
{
    int i;
    for (i = 0; i < CMD_COUNT; i+=1) {
        if (strcmp(cmd, CMD_NAME_LIST[i]) == 0) {
            return i;
        }
    }
    return -1;
}

/* execute the given command, passing an argument to it */
int call_cmd(char* cmd, char* arg) {
    int cmd_index;
    cmd_index = get_cmd_index(cmd);
    return CMD_LIST[cmd_index](arg);
}

/* read the command from standard input */
int read_cmd(char* cmd, char* arg) {
    print_prompt();
    return scanf("%s %s", cmd, arg);
}

/* print shell prompt to standard output */
void print_prompt(void) {
    printf("> ");
}

/******************************************************************************/
/* ╭──────────────────────────────────────────────────────────╮
 * │                IMPLEMENTATION OF COMMANDS                │
 * ╰──────────────────────────────────────────────────────────╯
*/

/* change file mode to 000 (very restrictive) */
int protege_pra_caramba(char* filepath)
{
    char mode[] = "0000";
    unsigned int mask = strtol(mode, 0, 8);
    return chmod(filepath, mask);
}

/* change file mode to 777 (very permissive) */
int libera_geral(char* filepath)
{
    char mode[] = "0777";
    unsigned int mask = strtol(mode, 0, 8);
    return chmod(filepath, mask);
}

/* runs a program on foreground and wait for it to complete */
int rode_veja(char* filepath)
{
    int exit_status, pid;
    char** argv, ** env;

    argv = NULL;
    env = NULL;
    pid = fork();

    /* this is the child process */
    /* execute program and quit */
    if (pid == 0) {
        exit(execve(filepath, argv, env));
    }

    /* this is the parent process */
    /* wait for child process to terminate, and try to get its exit status */
    wait(&exit_status);
    if (exit_status == -1) {
        printf("ERROR: erro ao rodar o programa %s\n", filepath);
    } else {
        /* the status is multiplied by 256, thus needs to convert it */
        exit_status /= 256;
        printf("=> programa '%s' retornou codigo %d\n", filepath, exit_status);
    }

    return 0;
}

/* runs a program on the background */
int rode(char* filepath)
{
    char** argv, ** env;
    int pid;

    argv = NULL;
    env = NULL;
    pid = fork();

    /* this is the child process */
    /* execute program and quit */
    if (pid == 0) {
        exit(execve(filepath, argv, env));
    }

    return 0;
}
