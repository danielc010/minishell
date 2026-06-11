/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes3.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/06/11 15:31:00 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/11 15:43:23 by dachin        ########   odam.nl         */
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

// Close all pipe fds in child after dup2 so parent is not hanging
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
