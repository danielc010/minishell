/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_parser.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: luimarti <luimarti@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/04/24 20:04:11 by luimarti      #+#    #+#                 */
/*   Updated: 2026/06/15 15:32:25 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
				return (-1);
			}
		}
		else if (current->type == WORD)
			i++;
		current = current->next;
	}
	return (i);
}

t_cmd	*init_cmd(void)
{
	t_cmd	*body;

	body = ft_calloc(1, sizeof(t_cmd));
	if (!body)
		return (NULL);
	body->args = NULL;
	body->redirs = NULL;
	body->next = NULL;
	return (body);
}

t_redir	*init_redir(void)
{
	t_redir	*current;

	current = ft_calloc(1, sizeof(t_redir));
	if (!current)
		return (NULL);
	current->type = 0;
	current->file = NULL;
	current->fd = 0;
	current->next = NULL;
	return (current);
}

void	free_array(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return ;
}

void	free_cmd(t_cmd *commands)
{
	t_cmd	*tmp;

	while (commands)
	{
		if (commands->redirs)
			free_redir(commands->redirs);
		free_array(commands->args);
		tmp = commands->next;
		free (commands);
		commands = tmp;
	}
	return ;
}
