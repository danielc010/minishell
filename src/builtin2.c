/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin2.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:58:04 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/05 08:08:18 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static	int	do_cd(t_shell *shell, char *path)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(oldpwd);
		return (1);
	}
	pwd = getcwd(NULL, 0);
	update_env(shell, "OLDPWD", oldpwd);
	update_env(shell, "PWD", pwd);
	free(oldpwd);
	free(pwd);
	return (0);
}

void	builtin_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!shell || !cmd)
		return ;
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	path = get_cd_path(shell, cmd);
	if (!path)
	{
		shell->exit_status = 1;
		return ;
	}
	shell->exit_status = do_cd(shell, path);
}
/* FIX: Added error handling for failed chdir() and too many arguments.
** Previously, cd silently returned exit code 0 on failure. Now it prints
** the appropriate error message and sets shell->exit_status, matching
** bash behaviour. Extracted do_cd() to handle the actual directory change
** and environment update, keeping builtin_cd() within norminette limits.
** Function was split into do_cd() and builtin_cd() to comply with
** norminette 25-line limit. */

void	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	int	i;

	if (!shell || !cmd)
		return ;
	ft_putendl_fd("exit", 2);
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	if (cmd->args[1])
	{
		i = 0;
		if (cmd->args[1][0] == '+' || cmd->args[1][0] == '-')
			i++;
		while (cmd->args[1][i])
		{
			if (!ft_isdigit(cmd->args[1][i]))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(cmd->args[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				exit(2);
			}
			i++;
		}
		exit(ft_atoi(cmd->args[1]) & 0xFF);
	}
	exit(shell->exit_status);
}

/* FIX: Added validation for exit arguments.
** Previously, builtin_exit did not validate its argument — ft_atoi("hello")
** returned 0 and the shell exited silently with code 0. Now we check:
** 1. Too many arguments (args[1] and args[2] exist) — print error, return
**    without exiting, set exit_status = 1.
** 2. Non-numeric argument — print "numeric argument required", exit(2).
** 3. Exit code is masked with 0xFF to keep it in the 0-255 range,
**    matching bash behaviour (e.g. exit 298 returns 42).
** ft_putendl_fd("exit") is always printed first, as bash does. */

// The additional == '=' check ensures key "FOX" doesn't trigger "FOXERA=..."
int	key_index(t_shell *shell, char *key)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(key);
	while (shell->env[i])
	{
		if ((!ft_strncmp(shell->env[i], key, j)) && (shell->env[i][j] == '='))
			return (i);
		++i;
	}
	return (-1);
}

// For each NAME, remove the corresponding variable or function. Shift everything
// left by one position then sets last entry to NULL. No reallocation of array.
void	builtin_unset(t_shell *shell, t_cmd *cmd)
{
	char	*key;
	int		index;

	if (!shell || !cmd || !cmd->args[1])
		return ;
	key = cmd->args[1];
	index = key_index(shell, key);
	if (index != -1)
	{
		free(shell->env[index]);
		while (shell->env[index])
		{
			shell->env[index] = shell->env[index + 1];
			++index;
		}
		shell->env[index] = NULL;
	}
}
