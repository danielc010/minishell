#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

void sigint_handler(int sig)
{
    (void)sig;
    printf("\n");  // New line after ^C
    rl_replace_line("", 0);  // Clear the line
    rl_on_new_line();  // Tell readline we're on a new line
    rl_redisplay();
}

int main(void)
{
    char *line;

    signal(SIGINT, sigint_handler);  // Handle Ctrl+C

    while (1)
    {
        line = readline("shell> ");
        if (line == NULL) // Handle EOF (Ctrl+D)
            break;

        if (strlen(line) > 0)
            add_history(line);

        printf("Executed: %s\n", line);
        free(line);
    }

    return (0);
}
