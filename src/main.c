#include "../include/minishell.h"

int	g_signal = 0;

int	main(int argc, char **argv, char **envp) // so we can access envp
{
	char *line;
	t_shell shell;

	(void)argc;
	(void)argv;
	dup_envp(&shell, envp);
	signals_interactive();
	while(1)
	{
		line = readline("shell> ");
		if (line == NULL) // EOF (ctrl-D)
		{
			break ;
		}
		if (g_signal == SIGINT)
		{
			write(1, "\n", 1);
			g_signal = 0;
			free(line);
			continue ;
		}
		if (ft_strlen(line) > 0) //add_history as long as something was typed
			add_history(line);
		// fork_and_exec(cmd, &shell); // placeholder to insrt cmd from parser output
		free(line); // manual free as readline is designed as such
	}

	return (0);
}
