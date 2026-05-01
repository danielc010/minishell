/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 14:48:45 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/01 17:52:04 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		free(redirs->file);
		tmp = redirs->next;
		free (redirs);
		redirs = tmp;
	}
	return ;
}

int	add_redir(t_redir *new, t_token *tokens, t_redir **head, t_redir **current)
{
	new->file = ft_strdup(tokens->next->value);
	if (!new->file)
	{
		free_redir(new);
		free_redir(*head);
		return (0);
	}
	if (!*head)
	{
		*head = new;
		*current = new;
	}
	else
	{
		(*current)->next = new;
		*current = new;
	}
	return (1);
}

t_redir_type	redir_type(t_token *tokens)
{
	t_redir_type	type;

	if (tokens->type == REDIR_IN)
		type = R_IN;
	else if (tokens->type == REDIR_OUT)
		type = R_OUT;
	else if (tokens->type == REDIR_APPEND)
		type = R_APPEND;
	else if (tokens->type == HEREDOC)
		type = R_HEREDOC;
	else
		type = -1;
	return (type);
}

int	fill_redir(t_token *tokens, t_redir **head, t_redir **current)
{
	t_redir	*new;

	new = NULL;
	if (!tokens->next)
	{
		ft_putstr_fd("Syntax error\n", 2);
		free_redir(*head);
		return (0);
	}
	new = init_redir();
	if (!new)
	{
		free_redir(*head);
		return (0);
	}
	new->type = redir_type(tokens);
	if (!add_redir(new, tokens, head, current))
		return (0);
	return (1);
}

t_redir	*build_redir(t_token *tokens)
{
	t_redir	*head;
	t_redir	*current;

	head = NULL;
	current = NULL;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
			|| tokens->type == REDIR_APPEND || tokens->type == HEREDOC)
		{
			if (!fill_redir(tokens, &head, &current))
				return (NULL);
			else
				tokens = tokens->next->next;
		}
		else
			tokens = tokens->next;
	}
	return (head);
}
