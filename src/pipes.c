/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 23:39:28 by dachin            #+#    #+#             */
/*   Updated: 2026/06/05 17:37:50 by luimarti         ###   ########.fr       */
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
	int	i;
	int	pid;
	int	status;

	i = 0;
	while (i < n)
	{
		pid = pids[i];
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
		++i;
	}
	signals_interactive();
	g_signal = 0;
}

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
			if (!ft_strncmp(cmd->args[0], "echo", 5))
				exit(ft_echo(cmd->args));
			if (!ft_strncmp(cmd->args[0], "pwd", 4))
				exit(ft_pwd());
			if (!ft_strncmp(cmd->args[0], "env", 4))
				exit(ft_env(shell));
			if (!ft_strncmp(cmd->args[0], "exit", 5))
			{
				if (cmd->args[1])
					exit(ft_atoi(cmd->args[1]));
				exit(ft_atoi("0"));
			}
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
			exec_child(cmd, shell);
			exit(EXIT_FAILURE);
		}
		else
		{
			ctx->pids[i] = pid;
			cmd = cmd->next;
			++i;
		}
	}
}

void	run_pipeline(t_shell *shell, t_cmd *cmd, int n)
{
	t_pipe_ctx	ctx;

	ctx.n = n;
	ctx.pipes = malloc(sizeof(int *) * (n - 1));
	if (!ctx.pipes)
		return ;
	if (init_pipes(ctx.pipes, n - 1) == -1)
	{
		free_pipes(ctx.pipes, n - 1);
		return ;
	}
	ctx.pids = malloc(sizeof(pid_t) * n);
	if (!ctx.pids)
	{
		free_pipes(ctx.pipes, n - 1);
		return ;
	}
	fork_children(shell, cmd, &ctx);
	close_all_pipes(ctx.pipes, n - 1);
	wait_all(shell, ctx.pids, n);
	free(ctx.pids);
	free_pipes(ctx.pipes, n - 1);
}

static int	run_builtin(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strncmp(cmd->args[0], "echo", 5))
		return (shell->exit_status = ft_echo(cmd->args), 1);
	if (!ft_strncmp(cmd->args[0], "pwd", 4))
		return (shell->exit_status = ft_pwd(), 1);
	if (!ft_strncmp(cmd->args[0], "env", 4))
		return (shell->exit_status = ft_env(shell), 1);
	if (!ft_strncmp(cmd->args[0], "cd", 3))
		return (builtin_cd(shell, cmd), 1);
	if (!ft_strncmp(cmd->args[0], "exit", 6))
		return (builtin_exit(shell, cmd), 1);
	if (!ft_strncmp(cmd->args[0], "export", 7))
		return (builtin_export(shell, cmd), 1);
	if (!ft_strncmp(cmd->args[0], "unset", 6))
		return (builtin_unset(shell, cmd), 1);
	return (0);
}

static void	exec_builtin_with_redirs(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		apply_redirs(cmd->redirs);
		if (!ft_strncmp(cmd->args[0], "echo", 5))
			exit(ft_echo(cmd->args));
		if (!ft_strncmp(cmd->args[0], "pwd", 4))
			exit(ft_pwd());
		if (!ft_strncmp(cmd->args[0], "env", 4))
			exit(ft_env(shell));
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
		if (!ft_strncmp(cmd->args[0], "exit", 5))
		{
			if (cmd->args[1])
				exit(ft_atoi(cmd->args[1]));
			exit(ft_atoi("0"));
		}
		exec_child(cmd, shell);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		signals_interactive();
	}
}

void	exec_pipeline(t_shell *shell)
{
	t_cmd	*cmd;
	int		n;

	cmd = shell->cmds;
	prepare_heredocs(cmd);
	n = count_cmds(cmd);
	if (n == 1)
	{
		if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
			return ;
		if (cmd->redirs)
		{
			exec_builtin_with_redirs(cmd, shell);
			return ;
		}
		if (!run_builtin(shell, cmd))
			fork_and_exec(shell->cmds, shell);
		return ;
	}
	run_pipeline(shell, cmd, n);
}
