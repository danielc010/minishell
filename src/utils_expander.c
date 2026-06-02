/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:02:08 by lucho             #+#    #+#             */
/*   Updated: 2026/05/14 14:29:37 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*find_value(char *name, t_shell *shell)
{
	int		i;
	char	*value;

	if (!name)
		return (NULL);
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], name, ft_strlen(name))
			&& shell->env[i][ft_strlen(name)] == '=')
		{
			value = ft_substr(shell->env[i], ft_strlen(name) + 1,
					ft_strlen(shell->env[i]) - (ft_strlen(name) + 1));
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

char	*append_char(char *str, char *result, int *i)
{
	char	*piece;
	char	*tmp;

	piece = ft_substr(str, *i, 1);
	tmp = ft_strjoin(result, piece);
	free (result);
	result = tmp;
	free (piece);
	return (result);
}

char	*expand_var(char *str, t_shell *shell, char *result, int *i)
{
	char	*var_name;
	char	*var_value;
	char	*tmp;

	var_name = get_var_name(&str[*i]);
	var_value = find_value(var_name, shell);
	if (!var_value)
		var_value = ft_strdup("");
	tmp = ft_strjoin(result, var_value);
	*i = *i + ft_strlen(var_name);
	free(result);
	result = tmp;
	free(var_name);
	free(var_value);
	return (result);
}

char	*dollar_handler(char *str, char *result, t_shell *shell, int *i)
{
	if (str[(*i) + 1] == '\0' || str[(*i) + 1] == ' ')
	{
		result = append_char(str, result, i);
		(*i)++;
		return (result);
	}
	(*i)++;
	if (str[*i] == '?')
		result = special_handler(result, shell, i);
	else
		result = expand_var(str, shell, result, i);
	return (result);
}
