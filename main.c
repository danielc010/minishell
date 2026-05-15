/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 13:45:01 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/15 23:14:24 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	char		*line;
	t_token		*tokens;
	t_cmd		*head;
	t_cmd		*current;
	t_redir		*redir;
	int i;

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
			head = parsing_core(tokens);
			expander(head, &shell);
			if (ft_strncmp(head->args[0], "echo", 4) == 0)
    			ft_echo(head->args);
			current = head;
			while (current)
			{
				i = 0;
				printf("--- Args ---\n");
				while (current->args && current->args[i])
				{
					printf("args[%d]: %s\n", i, current->args[i]);
					i++;
				}
				redir = current->redirs;
				while (redir)
				{
    				printf("redir type: %d, file: %s\n", redir->type, redir->file);
    				redir = redir->next;
				}
				current = current->next;
			}
			free_cmd(head);
			free_tokens(tokens);
		}
		free(line);
		line = readline("minishell>");
	}
	free_array(shell.env);
	return (0);
}
