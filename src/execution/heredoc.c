/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dachin <dachin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 00:00:00 by dachin            #+#    #+#             */
/*   Updated: 2026/06/12 00:00:00 by dachin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Reads one heredoc line using read() instead of readline so that ctrl-C
// (SIGINT) interrupts the read() syscall directly (returns -1, errno=EINTR)
// rather than relying on readline's signal handling. Returns NULL on both
// EOF (ctrl-D, ret==0) and SIGINT (ret==-1); caller checks g_signal to tell
// them apart.
static char	*read_line(void)
{
	char	*line;
	char	*tmp;
	char	c[2];
	int		ret;

	c[1] = '\0';
	line = ft_strdup("");
	if (!line)
		return (NULL);
	write(STDOUT_FILENO, "> ", 2);
	while (1)
	{
		ret = read(STDIN_FILENO, c, 1);
		if (ret != 1 || c[0] == '\n')
			break ;
		tmp = ft_strjoin(line, c);
		free(line);
		line = tmp;
		if (!line)
			return (NULL);
	}
	if (ret != 1)
		return (free(line), NULL);
	return (line);
}

// Len of line & len of delim to match to avoid "EOFEXTRA" triggering "EOF".
// We add a newline after writing line as read() strips newline from input.
int	process_heredoc_line(char *delim, int *pipe_fd)
{
	char	*line;

	line = read_line();
	if (g_signal == SIGINT)
	{
		free(line);
		g_signal = 0;
		return (-1);
	}
	if (line == NULL)
		return (0);
	if (ft_strlen(line) == ft_strlen(delim)
		&& !ft_strncmp(line, delim, ft_strlen(delim)))
	{
		free(line);
		return (0);
	}
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
	free(line);
	return (1);
}

// Close write end of pipe so reader does not wait forever for EOF from pipe.
int	setup_heredoc(char *delim)
{
	int		pipe_fd[2];
	int		x;

	pipe(pipe_fd);
	signals_waiting();
	while (1)
	{
		x = process_heredoc_line(delim, pipe_fd);
		if (x == -1)
		{
			signals_interactive();
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (-1);
		}
		if (x == 0)
			break ;
	}
	signals_interactive();
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
