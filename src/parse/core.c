/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 13:58:59 by luimarti          #+#    #+#             */
/*   Updated: 2026/06/17 10:14:31 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_cmd	*parse(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*head;

	tokens = lexer(line);
	if (!tokens)
		return (NULL);
	head = parsing_core(tokens);
	free_tokens(tokens);
	if (!head)
	{
		shell->exit_status = 2;
		return (NULL);
	}
	expander(head, shell);
	return (head);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->cmds = NULL;
	shell->env = copy_env(envp);
	if (!shell->env)
	{
		ft_putstr_fd("minishell: fatal: failed to copy environment\n", 2);
		exit(EXIT_FAILURE);
	}
	shell->exit_status = 0;
	shell->running = 1;
	return ;
}
