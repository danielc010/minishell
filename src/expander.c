/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 10:56:58 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/18 18:04:44 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char	*get_str(char *str, t_shell *shell)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i] != '\0')
		result = dispatcher(str, shell, result, &i);
	return (result);
}

void	expander(t_cmd *cmds, t_shell *shell)
{
	t_redir	*redir;
	char	*tmp;
	int		i;

	while (cmds)
	{
		i = 0;
		while (cmds->args[i])
		{
			tmp = cmds->args[i];
			cmds->args[i] = get_str(tmp, shell);
			free(tmp);
			i++;
		}
		redir = cmds->redirs;
		while (redir)
		{
			tmp = redir->file;
			redir->file = get_str(tmp, shell);
			free(tmp);
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
