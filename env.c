/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 10:58:25 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/18 17:33:41 by lucho            ###   ########.fr       */
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
