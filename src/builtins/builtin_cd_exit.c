/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 23:58:04 by dachin            #+#    #+#             */
/*   Updated: 2026/06/19 17:07:02 by luimarti         ###   ########.fr       */
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

int	digits_too_large(char *digits)
{
	char	*max;

	max = "9223372036854775807";
	if (ft_strlen(digits) > 19)
		return (1);
	if (ft_strlen(digits) == 19 && ft_strncmp(digits, max, 19) > 0)
		return (1);
	return (0);
}

// Exit code is masked (with & 0XFF) to the lowest 8 bits,
// keeping in 0-255 range as exit codes are a single byte.
void	validate_and_exit(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '+' || arg[0] == '-')
		i++;
	if (digits_too_large(arg + i))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
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
	if (!cmd->args[1])
		exit(shell->exit_status);
	if (is_valid_number(cmd->args[1]) && cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return ;
	}
	validate_and_exit(cmd->args[1]);
}
