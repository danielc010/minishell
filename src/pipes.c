/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:28 by dachin        #+#    #+#                 */
/*   Updated: 2026/05/27 21:06:26 by dachin        ########   odam.nl         */
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
			{
				redir->fd = setup_heredoc(redir->file);
			}
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

void	exec_pipeline(t_shell *shell)
{
	t_cmd	*cmd;
	int		n;

	cmd = shell->cmds;
	prepare_heredocs(cmd);
	n = count_cmds(cmd);
	if (n == 1)
	{
		if (!ft_strncmp(cmd->args[0], "cd", 3))
		{
			builtin_cd(shell, cmd);
			return ;
		}
		if (!ft_strncmp(cmd->args[0], "exit", 6))
		{
			builtin_exit(shell, cmd);
			return ;
		}
		if (!ft_strncmp(cmd->args[0], "export", 7))
		{
			builtin_export(shell, cmd);
			return ;
		}
		if (!ft_strncmp(cmd->args[0], "unset", 6))
		{
			builtin_unset(shell, cmd);
			return ;
		}
		fork_and_exec(shell->cmds, shell);
		return ;
	}
	run_pipeline(shell, cmd, n);
}
