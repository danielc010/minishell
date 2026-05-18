/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 20:04:11 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/18 17:25:48 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*body;

	body = malloc(sizeof(t_cmd));
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

	current = malloc(sizeof(t_redir));
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
