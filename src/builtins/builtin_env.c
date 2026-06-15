/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 10:58:25 by luimarti          #+#    #+#             */
/*   Updated: 2026/06/12 14:35:57 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**copy_env(char **envp)
{
	char	**env;
	int		n;

	n = 0;
	while (envp[n] != NULL)
		n++;
	env = ft_calloc(n + 1, sizeof(char *));
	if (!env)
		return (NULL);
	n = 0;
	while (envp[n] != NULL)
	{
		env[n] = ft_strdup(envp[n]);
		if (!env[n])
		{
			free_partial_env(env, n);
			return (NULL);
		}
		n++;
	}
	env[n] = NULL;
	return (env);
}

int	ft_env(t_shell *shell)
{
	char	**var;
	int		i;

	if (!shell)
		return (1);
	var = shell->env;
	i = 0;
	while (var[i])
	{
		printf("%s\n", var[i]);
		i++;
	}
	return (0);
}
