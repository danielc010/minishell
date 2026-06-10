/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_env_utils.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:07 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/10 17:10:55 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	replace_env_entry(char **entry, char *newval)
{
	char	*tmp;

	tmp = *entry;
	*entry = newval;
	free(tmp);
}

// Copies shell->env entries into env_new. The old array shell->env is freed
// but this does not affect the individual strings as they were copied out.
void	add_env(t_shell *shell, int i, char *new_env)
{
	char	**env_new;

	env_new = malloc((i + 2) * sizeof(char *));
	if (!env_new)
		return ;
	env_new[i + 1] = NULL;
	env_new[i] = new_env;
	--i;
	while (i >= 0)
	{
		env_new[i] = shell->env[i];
		--i;
	}
	free(shell->env);
	shell->env = env_new;
}

void	update_env(t_shell *shell, char *key, char *value)
{
	int		i;
	char	*keyequal;
	char	*keyequalvalue;

	if (!shell || !key || !value)
		return ;
	keyequal = ft_strjoin(key, "=");
	keyequalvalue = ft_strjoin(keyequal, value);
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], keyequal, ft_strlen(keyequal))
			|| (!ft_strncmp(shell->env[i], key, ft_strlen(key))
			&& shell->env[i][ft_strlen(key)] == '\0'))
		{
			replace_env_entry(&shell->env[i], keyequalvalue);
			free(keyequal);
			return ;
		}
		++i;
	}
	free(keyequal);
	add_env(shell, i, keyequalvalue);
}

// Skip past "KEY=" extract the pointer directly into the string at
// the value part.
// "PATH=/usr/bin" returns "/user/bin"
char	*get_env(t_shell *shell, char *key)
{
	int		i;
	char	*keyequal;
	int		len;

	if (!shell || !key)
		return (NULL);
	keyequal = ft_strjoin(key, "=");
	len = ft_strlen(keyequal);
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], keyequal, len))
		{
			free(keyequal);
			return (shell->env[i] + len);
		}
		++i;
	}
	free(keyequal);
	return (NULL);
}

// Sets the path for 3 cases:-
// cd alone: goes to HOME folder i.e. cd ~
// cd -: goes to previous directory i.e. OLDPWD
// cd xx: goes to directory ./xx
char	*get_cd_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!shell || !cmd)
		return (NULL);
	if (!cmd->args[1])
		path = get_env(shell, "HOME");
	else if (!ft_strncmp(cmd->args[1], "-", 2))
	{
		path = get_env(shell, "OLDPWD");
		if (path)
			printf("%s\n", path);
	}
	else
		path = cmd->args[1];
	return (path);
}
