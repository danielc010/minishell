/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/22 14:57:15 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/12 08:04:27 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	if (!ft_strncmp(cmd->args[0], "exit", 5))
		return (builtin_exit(shell, cmd), 1);
	if (!ft_strncmp(cmd->args[0], "export", 7))
		return (builtin_export(shell, cmd), 1);
	if (!ft_strncmp(cmd->args[0], "unset", 6))
		return (builtin_unset(shell, cmd), 1);
	return (0);
}

// Without signals_waiting(), the parent process is still using the
// interactive signal handler while waiting for the child. If you press
// Ctrl+C during a builtin with redirections (e.g. echo test > file takes
// a moment), the interactive handler fires — it calls rl_redisplay()
// which prints a new prompt. Then when waitpid returns and the main
// loop calls readline, it prints another prompt causing double-prompt
// bug. signals_waiting() swaps to the simpler handler that
// just sets g_signal and  writes \n, preventing the extra prompt.
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
		run_builtin_in_child(shell, cmd);
	}
	else
	{
		signals_waiting();
		waitpid(pid, &status, 0);
		handle_wait_status(shell, status);
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
		if (cmd->redirs)
		{
			exec_builtin_with_redirs(cmd, shell);
			return ;
		}
		if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
			return ;
		if (!run_builtin(shell, cmd))
			fork_and_exec(shell->cmds, shell);
		return ;
	}
	run_pipeline(shell, cmd, n);
}

// Set up array for correct number of pipes. Malloc failure is handled elsewhere
int	init_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			return (-1);
		}
		if (pipe(pipes[i]) == -1)
			return (-1);
		++i;
	}
	return (0);
}
