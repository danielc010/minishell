/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: luimarti <luimarti@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:28 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/12 09:20:29 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	prepare_heredocs(t_cmd *cmd)
{
	t_redir	*redir;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == R_HEREDOC)
				redir->fd = setup_heredoc(redir->file);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

void	wait_all(t_shell *shell, pid_t *pids, int n)
{
	int		i;
	pid_t	pid;
	int		status;

	i = 0;
	while (i < n)
	{
		pid = pids[i];
		waitpid(pid, &status, 0);
		handle_wait_status(shell, status);
		++i;
	}
	signals_interactive();
	g_signal = 0;
}

void	child_state_builtins(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strncmp(cmd->args[0], "cd", 3))
	{
		builtin_cd(shell, cmd);
		exit(shell->exit_status);
	}
	if (!ft_strncmp(cmd->args[0], "export", 7))
	{
		builtin_export(shell, cmd);
		exit(shell->exit_status);
	}
	if (!ft_strncmp(cmd->args[0], "unset", 6))
	{
		builtin_unset(shell, cmd);
		exit(shell->exit_status);
	}
}

void	run_builtin_in_child(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		exit(0);
	if (!ft_strncmp(cmd->args[0], "echo", 5))
		exit(ft_echo(cmd->args));
	if (!ft_strncmp(cmd->args[0], "pwd", 4))
		exit(ft_pwd());
	if (!ft_strncmp(cmd->args[0], "env", 4))
		exit(ft_env(shell));
	if (!ft_strncmp(cmd->args[0], "exit", 5))
	{
		builtin_exit(shell, cmd);
		exit(shell->exit_status);
	}
	child_state_builtins(shell, cmd);
	exec_child(cmd, shell);
	exit(EXIT_FAILURE);
}

// Each child calls close_all_pipes after dup2 as they are no longer needed
void	fork_children(t_shell *shell, t_cmd *cmd, t_pipe_ctx *ctx)
{
	int	i;
	int	pid;

	i = 0;
	while (i < ctx->n)
	{
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
		{
			setup_child_fds(ctx->pipes, i, ctx->n);
			close_all_pipes(ctx->pipes, ctx->n - 1);
			apply_redirs(cmd->redirs);
			run_builtin_in_child(shell, cmd);
		}
		else
		{
			ctx->pids[i] = pid;
			cmd = cmd->next;
			++i;
		}
	}
}
