/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:56:39 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/01 17:27:44 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	word_counter(t_token *tokens)
{
	int		i;
	t_token	*current;

	i = 0;
	current = tokens;
	while (current && current->type != PIPE)
	{
		if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
		{
			if (current->next)
				current = current->next;
			else
			{
				ft_putstr_fd("Syntax error\n", 2);
				return (-1); // Devolvemos -1 e lugar de 0 para distinguir entre error y el hehco de que no haya palabras.
			}
		}
		else if (current->type == WORD)
			i++;
		current = current->next;
	}
	return (i);
}

static int	fill_args(t_token *current, char **args, int *i)
{
	if (current->type == REDIR_IN || current->type == REDIR_OUT
		|| current->type == REDIR_APPEND || current->type == HEREDOC)
	{
		if (current->next)
			return (1);
		else
		{
			free_array(args);
			return (-1);
		}
	}
	else if (current->type == WORD)
	{
		args[*i] = ft_strdup(current->value);
		if (!args[*i])
			return (-1);
		(*i)++;
	}
	return (0);
}

char	**build_args(t_token *tokens, int words)
{
	char	**args;
	t_token	*current;
	int		i;
	int		x;

	args = malloc (sizeof(char *) * (words + 1));
	if (!args)
		return (NULL);
	current = tokens;
	i = 0;
	while (current && current->type != PIPE)
	{
		x = fill_args(current, args, &i);
		if (x == -1)
		{
			free_array(args);
			return (NULL);
		}
		else if (x == 1)
			current = current->next;
		current = current->next;
	}
	args[i] = NULL;
	return (args);
}

int	if_pipe(t_token *tokens, t_cmd **command, t_cmd *head, int words)
{
	t_cmd	*second;

	if (tokens->type == PIPE)
	{
		second = init_cmd();
		words = word_counter(tokens->next);
		if (words == -1)
		{
			free_cmd(head);
			return (0);
		}
		second->args = build_args(tokens->next, words);
		second->redirs = build_redir(tokens->next);
		(*command)->next = second;
		*command = second;
	}
	return (1);
}

t_cmd	*parsing_core(t_token *tokens)
{
	t_cmd	*command;
	t_cmd	*head;
	int		words;

	if (!tokens)
		return (NULL);
	command = init_cmd();
	words = word_counter(tokens);
	if (words == -1)
	{
		free_cmd(command);
		return (NULL);
	}
	command->args = build_args(tokens, words);
	command->redirs = build_redir(tokens);
	head = command;
	while (tokens)
	{
		if (if_pipe(tokens, &command, head, words) == 0)
			return (NULL);
		tokens = tokens->next;
	}
	return (head);
}
