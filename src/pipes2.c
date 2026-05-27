/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/22 14:57:15 by dachin        #+#    #+#                 */
/*   Updated: 2026/05/22 15:00:55 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	return 0;
}

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
	while(i < n)
	{
		free(pipes[i]);
		++i;
	}
	free(pipes);
}

