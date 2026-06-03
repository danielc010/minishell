/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   core.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: luimarti <luimarti@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/21 13:58:59 by luimarti      #+#    #+#                 */
/*   Updated: 2026/06/03 15:26:23 by dachin        ########   odam.nl         */
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
		return (NULL);
	expander(head, shell);
	return (head);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->cmds = NULL;
	shell->env = copy_env(envp);
	shell->exit_status = 0;
	shell->running = 1;
	return ;
}
