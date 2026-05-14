/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 14:04:51 by lucho             #+#    #+#             */
/*   Updated: 2026/04/25 13:47:26 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*init_redirs(void)
{
	t_redir	*current;

	current = malloc(sizeof(t_redir));
	if (!current)
		return (NULL);
	current->type = 0;
	current->file = NULL;
	current->fd = -1;
	current->next = NULL;
	return (current);
}

t_redir	*build_redir(t_token *tokens)
{
	t_redir	*head;
	t_redir	*current;

	head = NULL;
	current = NULL;
	while (tokens && tokens->type != PIPE)
	{
		// aquí va la lógica
		tokens = tokens->next;
	}
	return (head);
}

if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
