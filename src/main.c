#include "../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	shell;

	(void)argc;
	(void)argv;
	dup_envp(&shell, envp);
	signals_interactive();
	rl_catch_signals = 0;
	while (1)
	{
		line = readline("shell> ");
		if (line == NULL)
		{
			if (g_signal == SIGINT)
			{
				g_signal = 0;
				continue ;
			}
			break ;
		}
		if (ft_strlen(line) > 0)
			add_history(line);
		exec_pipeline(&shell);
		free(line);
	}
	return (0);
}