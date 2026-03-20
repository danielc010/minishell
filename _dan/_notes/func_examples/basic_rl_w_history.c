#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
	char *line;
	int line_count = 0;

	while (1)
	{
		line = readline("shell> ");
		if (line == NULL) // Ctrl+D pressed
		{
			printf("\nExiting...\n");
			break ;
		}
		if (strlen(line) > 0)
			add_history(line); // Add to history only if non-empty
		printf("You typed (%d): %s\n", ++line_count, line);
        free(line);  // IMPORTANT: readline allocates memory
	}
	return (0);
}
