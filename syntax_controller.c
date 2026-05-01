/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_controller.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:52:34 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/01 19:48:06 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipes(t_token *tokens)
{
	if (tokens->type == PIPE)
	{
		ft_putstr_fd("Syntax error\n", 2);
		return (0);
	}
	while (tokens)
	{
		if (tokens->type == PIPE && tokens->next == NULL)
		{
			ft_putstr_fd("Syntax error\n", 2);
			return (0);
		}
		if (tokens->next && tokens->type == PIPE && tokens->next->type == PIPE)
		{
			ft_putstr_fd("Syntax error\n", 2);
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}

static int	is_redir(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

int	check_redirs(t_token *tokens)
{
	while (tokens)
	{
		if (is_redir(tokens->type))
		{
			if (!tokens->next || is_redir(tokens->next->type)
				|| tokens->next->type == PIPE)
			{
				ft_putstr_fd("Syntax error\n", 2);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

int	syntax_controller(t_token *tokens)
{
	if (!tokens)
		return (0);
	if (!check_pipes(tokens))
		return (0);
	if (!check_redirs(tokens))
		return (0);
	return (1);
}
