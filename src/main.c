/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 23:39:19 by dachin            #+#    #+#             */
/*   Updated: 2026/06/05 17:45:47 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	signals_interactive();
	rl_catch_signals = 0;
	while (shell.running)
	{
		line = readline("minishell> ");
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
		shell.cmds = parse(line, &shell);
		if (shell.cmds)
		{
			exec_pipeline(&shell);
			free_cmd(shell.cmds);
			shell.cmds = NULL;
		}
		free(line);
	}
	free_array(shell.env);
	return (0);
}
