/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucho <lucho@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 23:39:14 by dachin            #+#    #+#             */
/*   Updated: 2026/06/05 00:54:24 by lucho            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Finds all the PATH= in env, splitting on : and returns the array
char	**extract_path_to_array(t_shell *shell)
{
	int		i;
	char	**paths_array;

	if (!shell)
		return (NULL);
	i = 0;
	paths_array = NULL;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "PATH=", 5) == 0)
		{
			paths_array = ft_split(shell->env[i] + 5, ':');
			break ;
		}
		++i;
	}
	return (paths_array);
}

// if ((access(full_path, X_OK)) == 0): we found the executable, skip to
// last return statement
// if (paths_array[i] == NULL): command is not found
char	*find_in_paths(char **paths, char *with_slash)
{
	char	*full_path;
	int		i;

	full_path = NULL;
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], with_slash);
		if ((access(full_path, X_OK)) == 0)
			break ;
		else
		{
			free(full_path);
			full_path = NULL;
		}
		++i;
	}
	return (full_path);
}

char	*resolve_path(char *cmd, t_shell *shell)
{
	char	*with_slash;
	char	*full_path;
	char	**paths_array;
	DIR		*dir;

	if (!cmd || !shell)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
		{
			dir = opendir(cmd);
			if (dir)
			{
				closedir(dir);
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putstr_fd(": Is a directory\n", 2);
				exit(126);
			}
			return (ft_strdup(cmd));
		}
		if (access(cmd, F_OK) == 0)
		{
			perror("minishell");
			exit(126);
		}
		return (NULL);
	}
	paths_array = extract_path_to_array(shell);
	if (!paths_array)
		return (NULL);
	with_slash = ft_strjoin("/", cmd);
	full_path = find_in_paths(paths_array, with_slash);
	if (full_path == NULL)
		return (free(with_slash), free_array(paths_array), NULL);
	return (free(with_slash), free_array(paths_array), full_path);
}
/* FIX (lines 74 - 78): Added access(cmd, F_OK) check after access(cmd, X_OK) fails.
** Previously, when a file existed but had no execute permissions,
** resolve_path returned NULL and exec_child printed "command not found"
** with exit code 127. Now we distinguish between a missing file (NULL,
** exit 127) and an existing file without execute permissions (exit 126),
** matching bash behaviour. */

/* FIX 2: Added directory detection in resolve_path() using opendir().
** Previously, when a user tried to execute a directory (e.g. ./test_files
** or $PWD which expands to a path), access(cmd, X_OK) returned 0 because
** directories have execute permission by default. This caused resolve_path()
** to return the path, exec_child() to call execve(), which failed silently
** with exit code 1 instead of the correct 126.
** Now we use opendir() to check if the path is a directory before returning
** it. If it is, we print the correct error message and exit with code 126,
** matching bash behaviour. */

void	exec_child(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = resolve_path(cmd->args[0], shell);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->args[0], 2);
		exit(127);
	}
	signals_child();
	apply_redirs(cmd->redirs);
	execve(path, cmd->args, shell->env);
	free(path);
}

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
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
		signals_interactive();
	}
}
