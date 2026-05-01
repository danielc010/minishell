/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 14:47:16 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/01 18:13:46 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **envp)
{
	char	**env;
	int		n;

	n = 0;
	while (envp[n] != NULL)
		n++;
	env = malloc(sizeof(char *) * (n + 1));
	if (!env)
		return (NULL);
	n = 0;
	while (envp[n] != NULL)
	{
		env[n] = ft_strdup(envp[n]);
		if (!env[n])
		{
			free_array(env);
			return (NULL);
		}
		n++;
	}
	env[n] = NULL;
	return (env);
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
