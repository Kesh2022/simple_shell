#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 64

/* Function to tokenize the input command */
void tokenize_command(char *command, char **args)
{
    char *token;
    int i = 0;

    token = strtok(command, " \t\n\r\a");
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n\r\a");
    }
    args[i] = NULL;
}

/* Function to handle signals */
void signal_handler(int signum __attribute__((unused)))
{
    /* Handle signals as needed */
}

int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGUMENTS];
    pid_t child_pid;
    int status;

    /* Set up signal handler */
    signal(SIGINT, signal_handler);

    while (1)
    {
        /* Print shell prompt */
        printf("SimpleShell> ");
        fflush(stdout);

        /* Read command from user */
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
        {
            /* End of input, exit the shell */
            printf("\n");
            break;
        }

        /* Tokenize the command into arguments */
        tokenize_command(command, args);

        /* Check for built-in commands */
        if (strcmp(args[0], "exit") == 0)
        {
            /* Exit the shell */
            printf("Exiting SimpleShell.\n");
            break;
        }

        /* Fork a child process to execute the command */
        child_pid = fork();

        if (child_pid < 0)
        {
            perror("Fork error");
        }
        else if (child_pid == 0)
        {
            /* Child process */

            /* Execute the command */
            if (execvp(args[0], args) == -1)
            {
                perror("SimpleShell");
                _exit(EXIT_FAILURE);
            }

        }
        else
        {
            /* Parent process */

            /* Wait for the child process to finish */
            waitpid(child_pid, &status, 0);

            /* Check if child process exited normally */
            if (WIFEXITED(status))
            {
                printf("Child process exited with status %d.\n", WEXITSTATUS(status));
            }
            else
            {
                printf("Child process terminated abnormally.\n");
            }
        }
    }

    return 0;
}

