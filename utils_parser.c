/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 20:04:11 by luimarti          #+#    #+#             */
/*   Updated: 2026/04/24 20:04:29 by luimarti         ###   ########.fr       */
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
