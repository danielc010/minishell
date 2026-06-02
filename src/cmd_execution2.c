/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_execution2.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/22 12:19:00 by dachin        #+#    #+#                 */
/*   Updated: 2026/05/22 14:26:48 by dachin        ########   odam.nl         */
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
		if (redir->type == R_IN || redir->type == R_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else if (redir->type == R_OUT || redir->type == R_APPEND)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}

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
