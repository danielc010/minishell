/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 14:04:51 by lucho             #+#    #+#             */
/*   Updated: 2026/05/14 14:43:47 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
