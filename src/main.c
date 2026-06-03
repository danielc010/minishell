/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:19 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/01 17:14:52 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;


// Reset g_signal = 0 so the next iteraction doesn't think another
// Ctrl + c happened
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;

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
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
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



