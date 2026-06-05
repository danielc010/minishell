/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin2.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:58:04 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/05 11:01:00 by dachin        ########   odam.nl         */
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

// Exit code is masked (with & 0XFF) to the lowest 8 bits,
// keeping in 0-255 range as exit codes are a single byte.
static void	validate_and_exit(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		i++;
	}
	exit(ft_atoi(arg) & 0xFF);
}

void	builtin_exit(t_shell *shell, t_cmd *cmd)
{
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
		validate_and_exit(cmd->args[1]);
	exit(shell->exit_status);
}

