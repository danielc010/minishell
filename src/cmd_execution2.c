/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 12:19:00 by dachin            #+#    #+#             */
/*   Updated: 2026/06/02 17:00:20 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
/* FIX (lines 39-43): Added fd == -1 check after open_redir_fd().
** When open() fails (e.g. permission denied, file not found),
** it returns -1. Without this check, -1 was passed to dup2() and
** close(), causing the program to continue silently instead of
** reporting the error. Now we print the system error with perror()
** and exit with code 1, matching bash behaviour. */

int	handle_heredoc_signal(void)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		return (1);
	}
	return (0);
}

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
		if (!ft_strncmp(line, delim, ft_strlen(delim)))
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
