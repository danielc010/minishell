/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin2.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:58:04 by dachin        #+#    #+#                 */
/*   Updated: 2026/05/21 00:07:00 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	if (!shell || !cmd)
		return ;
	ft_putendl_fd("exit", 2);
	if (cmd->args[1])
		exit(ft_atoi(cmd->args[1]));
	else
		exit(shell->exit_status);
}

