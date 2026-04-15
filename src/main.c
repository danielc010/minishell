#include "../include/minishell.h"

int	g_signal = 0;

// int	main(int argc, char **argv, char **envp) // we will need this later
int	main(void)
{
	char *line;

	while(1)
	{
		line = readline("shell> ");
		if (line == NULL)
		{
			break ;
		}
		if (ft_strlen(line) > 0) //add_history as long as something was typed
			add_history(line);
		free(line); // manual free as readline is designed as such
	}
	return (0);
}
