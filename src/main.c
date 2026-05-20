/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:19 by dachin        #+#    #+#                 */
/*   Updated: 2026/05/21 00:34:05 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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

	// By default, readline installs its own signal handlers for SIGINT
	// (and others). When readline catches SIGINT, it does its own cleanup
	//  — but that interferes with your custom signal handling in
	//  signals_interactive().

	// Setting rl_catch_signals = 0 tells readline: "don't touch signal
	// handlers, I'll manage them myself." That way, when Ctrl+C is pressed,
	// your handle_signal in signals.c runs instead of readline's default behavior.
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



