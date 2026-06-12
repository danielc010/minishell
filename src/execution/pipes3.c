/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes3.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/11 15:31:00 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/12 10:05:45 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Set up child fds for each child process. The first child's stdin is untouched
void	setup_child_fds(int **pipes, int i, int n)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < n - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
}

// Close all pipe fds
void	close_all_pipes(int **pipes, int n)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		++i;
	}
}

void	free_pipes(int **pipes, int n)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < n)
	{
		free(pipes[i]);
		++i;
	}
	free(pipes);
}

// Exit status and signals:
// POSIX shell convention that when a processs is killed by a signal,
// WIFEXITED is false - process never called exit and there is no normal exit
// code. Shell needs to encode which signal killed it, so 128 + signal number,
// keeping it above normal exit code range (0-127) so it can be distinguished.
// Ctrl + C -> SIGINT (2) -> $? = 130
// Ctrl + \ -> SIGQUIT (3) -> $? = 131. Ignored by interactive shells, only
// has effect on a running child, terminating the process, generates a core dump
// Ctrl + D -> EOF, no signal.
void	handle_wait_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		shell->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
	}
}
