/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 18:43:43 by luimarti          #+#    #+#             */
/*   Updated: 2026/04/24 15:27:30 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_operator_token(t_lexer *lx, t_token_type type, char *value)
{
	lx->tok = new_token(type, value);
	if (!lx->tok)
		return (0);
	add_token_back(&lx->head, lx->tok);
	return (1);
}

int	is_separator(char c)
{
	if (c == ' ' || c == '\t' || c == '|' || c == '<'
		|| c == '>' || c == '\0')
		return (1);
	return (0);
}

t_token	*new_token(t_token_type type, char *value)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

void	add_token_back(t_token **head, t_token *new)
{
	t_token	*current;

	if (*head == NULL)
	{
		*head = new;
		return ;
	}
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new;
	}
	return ;
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head != NULL)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
	return ;
}
