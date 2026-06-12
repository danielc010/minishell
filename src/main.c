/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: luimarti <luimarti@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:19 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/12 06:00:12 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Global variables in C are zero-initialised by default
volatile sig_atomic_t	g_signal;

static void	shell_loop(t_shell *shell)
{
	char	*line;

	while (shell->running)
	{
		line = readline("minishell> ");
		if (g_signal == SIGINT)
			shell->exit_status = 130;
		g_signal = 0;
		if (line == NULL)
		{
			ft_putendl_fd("exit", 2);
			break ;
		}
		if (ft_strlen(line) > 0)
			add_history(line);
		shell->cmds = parse(line, shell);
		if (shell->cmds)
		{
			exec_pipeline(shell);
			free_cmd(shell->cmds);
			shell->cmds = NULL;
		}
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	signals_interactive();
	rl_catch_signals = 0;
	shell_loop(&shell);
	free_array(shell.env);
	return (shell.exit_status);
}
