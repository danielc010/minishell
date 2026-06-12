/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 12:19:00 by dachin            #+#    #+#             */
/*   Updated: 2026/06/12 17:32:14 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// When a child process finishes, it doesnt disapppear. It stays in the
// process as a zombie proesss. waiting for its parent to collect
// its exit status with waitpid for clean up, otherwise there will be
// process table leak where no new processes can be created.
// signals_waiting:
// While parent sits in waitpid, it's still a live process. Ctrl+C sends SIGINT
// to the entire foreground process group - both child and parent receive it
// simultaneously. If parent still has interactive signal handler active, the
// handler calls readline internals to redraw the prompt. When waitpid returns
// and shell_loop calls readline again, it draws a prompt a second time.
// signals_waiting() swaps to a simpler handler that only sets g_signal and
// writes \n, with no readline calls. After waitpid returns, signals_interactive
// restores the full handler.
// waitpid: fills status with a packed integeter encoding what happened to child
// man waitpid
void	fork_and_exec(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !shell)
		return ;
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
		signals_waiting();
		waitpid(pid, &status, 0);
		handle_wait_status(shell, status);
		signals_interactive();
	}
}

// Helper that takes a redirection and returns an open file descriptor
// appropriate for its type — read-only for R_IN, write/create/truncate for
// R_OUT, write/create/append for R_APPEND, or the already-prepared pipe fd for
// R_HEREDOC. It just opens the right fd; it doesn't do the actual redirecting.
int	open_redir_fd(t_redir *redir)
{
	int	fd;

	fd = 0;
	if (redir->type == R_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == R_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == R_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == R_HEREDOC)
		fd = redir->fd;
	return (fd);
}

// Connects the fd to stdin or stdout. Recap dup2 points arg2 to arg1.
// dup2(fd, STDIN_FILENO makes STDIN_FILENO (fd 0) point to same file/pipe as fd
// so any future read from stdin reads from your file/pipe instead.
// dup2(fd, STDOUT_FILENO makes STDOUT_FILENO (fd 1) point to same file/pipe as
// fd so any future write to stdout writes to your file/pipe instead.
void	apply_redirs(t_redir *redir)
{
	int	fd;

	fd = 0;
	while (redir)
	{
		fd = open_redir_fd(redir);
		if (fd == -1)
		{
			perror("minishell");
			exit(1);
		}
		if (redir->type == R_IN || redir->type == R_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else if (redir->type == R_OUT || redir->type == R_APPEND)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}

// Reset signal flag so the next iteraction doesn't think another
// Ctrl + C happened
int	handle_heredoc_signal(void)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		return (1);
	}
	return (0);
}

// Len of line & len of delim  to match to avoid "EOFEXTRA" triggering "EOF"
// We add a newline after writing line as readline strips newline from input
// Close write end of pipe so reader does not wait forever for EOF from pipe
int	setup_heredoc(char *delim)
{
	int		pipe_fd[2];
	char	*line;

	pipe(pipe_fd);
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			if (handle_heredoc_signal())
				continue ;
			break ;
		}
		if (!ft_strncmp(line, delim, ft_strlen(delim))
			&& ft_strlen(line) == ft_strlen(delim))
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
