/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 10:56:58 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/05 22:43:45 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_value(char *name, char **env)
{
	int		i;
	char	*value;

	if (!name)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], name, ft_strlen(name))
			&& env[i][ft_strlen(name)] == '=')
		{
			value = ft_substr(env[i], ft_strlen(name) + 1,
					ft_strlen(env[i]) - (ft_strlen(name) + 1));
			return (value);
		}
		i++;
	}
	return (NULL);
}

char	*get_var_name(char *str)
{
	int		i;
	char	*name;

	i = 0;
	if (!str)
		return (NULL);
	while (ft_isalpha(str[i])
		|| ft_isdigit(str[i]) || str[i] == '_')
		i++;
	name = ft_substr(str, 0, i);
	if (!name)
		return (NULL);
	return (name);
}
