/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 13:45:01 by luimarti          #+#    #+#             */
/*   Updated: 2026/04/21 16:41:34 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	char		*line;
	t_token		*tokens;

	(void)argc;
	(void)argv;
	shell.cmds = NULL;
	shell.env = copy_env(envp);
	shell.exit_status = 0;
	shell.running = 1;
	line = readline("minishell>");
	while (shell.running == 1 && line != NULL)
	{
		if (line[0])
		{
			tokens = lexer(line);
			add_history(line);
			free_tokens(tokens);
		}
		free(line);
		line = readline("minishell>");
	}
	free_array(shell.env);
	return (0);
}
