/* Compile com:
 *   cc -Wpedantic -Wall -Wextra -ansi mac422shell.c -o mac422shell
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>  /* chmod system call */
#include <unistd.h>    /* fork/execve system calls */
#include <sys/wait.h>  /* wait system call */

#define PROTEGE  "protegepracaramba"
#define LIBERA   "liberageral"
#define RODEVEJA "rodeveja"
#define RODE     "rode"
#define EXITCMD  "exit"
#define MAXCMD   32
#define MAXLINE  512

#define MIN(x, y) (x > y ? x : y)
#define printErrnoString(message) fprintf(stderr, message ": %s\n", strerror(errno))

void printString (char *s)
{
    size_t i = 0;
    while (s[i] != 0)
        fprintf(stderr, "%c,", s[i++]);
    fputs("", stderr);
}

/* This function implements the 'protegepracaramba' shell's command. */
void protege(char *line)
{
    int status = 0;
    status = chmod(line, 0000);
    if (status == -1)
        printErrnoString("Chmod error");
}

/* This function implements the 'liberageral' shell's command. */
void libera(char *line)
{
    int status = 0;
    status = chmod(line, S_IRWXU|S_IRWXG|S_IRWXO);
    if (status == -1)
        printErrnoString("Chmod error");
}

/* This function implements the 'rodeveja' shell's command. */
void rodeVeja(char *line)
{
    int status;
    pid_t child;
    char *argv[] = {NULL, NULL}, *env[] = {NULL};
    argv[0] = line;
    child = fork();
    if (child == -1)
        printErrnoString("Fork error");
    else if (child == 0) {
        status = execve(line, argv, env);
        if (status == -1) { /* redundante */
            printErrnoString("Execve error");
            exit(EXIT_FAILURE);
        }
    }
    else {
        child = waitpid(child, &status, 0);
        if (child == -1) {
            printErrnoString("Wait error");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "\n=> programa '%s' retornou com codigo %d\n", line, WEXITSTATUS(status));
    }
}

/* This function implements the 'rode' shell's command. */
void rode(char *line)
{
    int status;
    char *argv[] = {NULL, NULL}, *env[] = {NULL};
    argv[0] = line;

    status = fork();
    if (status == -1)
        printErrnoString("Fork error");
    else if (status == 0) {
        /* processo filho */
        close(0);
        status = execve(line, argv, env);
        if (status == -1) { /* redundante */
            printErrnoString("Execve error");
            exit(EXIT_FAILURE);
        }
    }
    else {
        /* processo pai */
    }
}

int charIsNotSpace(char c)
{
    return c != ' ' && c != '\n' && c != 0;
}

int charIsNotEnd(char c)
{
    return c != '\n' && c != 0;
}

/* This function reads stream until it finds an EOF or a newline, and stores at
 * most MAXLINE-1 characters in buffer.  It returns the number of characters
 * read.
 */
int getLine(char *buffer, FILE *stream)
{
    int size = 0;

    if (fgets(buffer, MAXLINE, stream) == NULL)
        return 0;
    while (charIsNotEnd(buffer[size])) size++;
    buffer[size] = 0;
    return size;
}

void printPrompt(const char *prompt, FILE *stream)
{
    fputs(prompt, stream);
}

/* This function copies the first word (contiguous sequence of nonspace
 * characters) of 'buffer' to 'command'.
 */
int copyCommandFromBuffer(char *command, char *buffer, int maxLength)
{
    int i = 0;
    for (i = 0; i < maxLength-1 && charIsNotSpace(buffer[i]); i++)
        command[i] = buffer[i];
    command[i] = 0;
    return i;
}

/* This function calls the function corresponding to 'command' argument string
 * and passes 'argline' as parameter to it.
 */
int executeCommand(char *command, char *argline)
{
    if (strcmp(command, PROTEGE) == 0)
        protege(argline);
    else if (strcmp(command, LIBERA) == 0)
        libera(argline);
    else if (strcmp(command, RODEVEJA) == 0)
        rodeVeja(argline);
    else if (strcmp(command, RODE) == 0)
        rode(argline);
    else if (strcmp(command, EXITCMD) == 0)
        return -1;
    else {
        return 0;
    }
    return 1;
}


int main()
{
    char buffer[MAXLINE+1], command[MAXCMD+1];
    int status, i, minSize;

    while (1) {
        printPrompt("\n$ ", stderr);
        status = getLine(buffer, stdin);  

        if (status == 0) break;

        minSize = MIN(status, MAXCMD);
        i = copyCommandFromBuffer(command, buffer, minSize);
        if (i >= minSize) {
            fputs("Unknown command.\n", stderr);
            continue;
        }
        while (i < status && buffer[i] == ' ') i++;
        status = executeCommand(command, &(buffer[i]));
        if (status == 0) {
            if (command[0] != 0) 
                fputs("Unknown command.", stderr);
            continue;
        }
        else if (status == -1)
            exit(EXIT_SUCCESS);
    }

    return 0;
}
