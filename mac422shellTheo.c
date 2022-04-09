/* Compile com:
 *   cc -Wpedantic -Wall -Wextra mac422shell.c -o mac422shell
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
    puts("");
}

void protege(char *line)
{
    int status = 0;
    status = chmod(line, 0000);
    if (status == -1)
        printErrnoString("Chmod error");
}

void libera(char *line)
{
    int status = 0;
    status = chmod(line, S_IRWXU|S_IRWXG|S_IRWXO);
    if (status == -1)
        printErrnoString("Chmod error");
}

void rodeVeja(char *line)
{
    int status;
    pid_t child;
    char *argv[] = {line, NULL}, *env[] = {NULL};
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

void rode(char *line)
{
    int status;
    char *argv[] = {line, NULL}, *env[] = {NULL};

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

int charIsNotSpace(char c) {
    return c != ' ' && c != '\n' && c != 0;
}

int charIsNotEnd(char c) {
    return c != '\n' && c != 0;
}

int getLine(char *buffer, FILE *fd)
{
    int size = 0;

    if (fgets(buffer, MAXLINE, fd) == NULL)
        return 0;
    while (charIsNotEnd(buffer[size])) size++;
    buffer[size] = 0;
    return size;
}

int main()
{
    char buffer[MAXLINE+1], command[MAXCMD+1];
    int status, i, minSize;

    while (1) {
        fputs("\n$ ", stderr);
        status = getLine(buffer, stdin);  

        if (status == 0) break;

        minSize = MIN(status, MAXCMD);
        for (i = 0; i < minSize && charIsNotSpace(buffer[i]); i++)
            command[i] = buffer[i];
        if (i >= minSize) {
            fputs("Unknown command.\n", stderr);
            continue;
        }
        command[i] = 0;
        while (i < status && buffer[i] == ' ') i++;

        if (strcmp(command, PROTEGE) == 0)
            protege(&(buffer[i]));
        else if (strcmp(command, LIBERA) == 0)
            libera(&(buffer[i]));
        else if (strcmp(command, RODEVEJA) == 0)
            rodeVeja(&(buffer[i]));
        else if (strcmp(command, RODE) == 0)
            rode(&(buffer[i]));
        else if (strcmp(command, EXITCMD) == 0)
            exit(EXIT_SUCCESS);
        else {
            if (command[0] != 0) 
                fputs("Unknown command.", stderr);
            continue;
        }
    }

    return 0;
}
