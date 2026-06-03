/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 13:45:01 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/21 14:25:31 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	char		*line;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	line = readline("minishell>");
	while (shell.running == 1 && line != NULL)
	{
		if (line[0])
			add_history(line);
		shell.cmds = parse(line, &shell);
		// if (shell.cmds)
    	// 	exec_pipeline(&shell);
		free_cmd(shell.cmds);
		free(line);
		line = readline("minishell>");
	}
	free_array(shell.env);
	return (0);
}
