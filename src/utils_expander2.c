/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expander2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 22:00:59 by lucho             #+#    #+#             */
/*   Updated: 2026/06/08 22:02:13 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*double_quote_handler(char *str, char *result, t_shell *shell, int *i)
{
	(*i)++;
	while (str[*i] != '"' && str[*i] != '\0')
	{
		if (str[*i] == '$')
		{
			(*i)++;
			if (str[*i] == '?')
				result = special_handler(result, shell, i);
			else if ((ft_isalpha(str[*i]) || ft_isdigit(str[*i])
					|| str[*i] == '_'))
				result = expand_var(str, shell, result, i);
			else
			{
				(*i)--;
				result = append_char(str, result, i);
				(*i)++;
			}
		}
		else
		{
			result = append_char(str, result, i);
			(*i)++;
		}
	}
	return (result);
}

char	*special_handler(char *result, t_shell *shell, int *i)
{
	char	*tmp;
	char	*var_value;

	var_value = ft_itoa(shell->exit_status);
	tmp = ft_strjoin(result, var_value);
	free(result);
	result = tmp;
	free(var_value);
	(*i)++;
	return (result);
}

char	*dispatcher(char *str, t_shell *shell, char *result, int *i)
{
	if (str[*i] == '\'')
	{
		(*i)++;
		while (str[*i] != '\'' && str[*i] != '\0')
		{
			result = append_char(str, result, i);
			(*i)++;
		}
		(*i)++;
	}
	else if (str[*i] == '"')
	{
		result = double_quote_handler(str, result, shell, i);
		(*i)++;
	}
	else if (str[*i] == '$')
		result = dollar_handler(str, result, shell, i);
	else
	{
		result = append_char(str, result, i);
		(*i)++;
	}
	return (result);
}
