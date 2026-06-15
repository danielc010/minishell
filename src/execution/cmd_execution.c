/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_execution.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:14 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/15 16:13:10 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Finds all the PATH= in env, splitting on : and returns the array
// PATH is environment variable telling OS where to search for exec programs
// and commands. The folders are delimited by :
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
// FIX: Added skipping access in case full_path = NULL
char	*find_in_paths(char **paths, char *with_slash)
{
	char	*full_path;
	int		i;

	full_path = NULL;
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(paths[i], with_slash);
		if (full_path)
		{
			if ((access(full_path, X_OK)) == 0)
				break ;
			else
			{
				free(full_path);
				full_path = NULL;
			}
		}
		++i;
	}
	return (full_path);
}

// Handles cmds on explicit path (starting with / or ./).
// Checks in order:
// not executable but exists -> permission denied (126) or not found (127)
// executable + is a directory -> Is a directory (126)"
// executable + is a file -> return the path
static char	*resolve_direct(char *cmd)
{
	DIR	*dir;

	if (access(cmd, X_OK) != 0)
	{
		if (access(cmd, F_OK) == 0)
		{
			perror("minishell");
			exit(126);
		}
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putendl_fd(": No such file or directory", 2);
		exit(127);
	}
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

char	*resolve_path(char *cmd, t_shell *shell)
{
	char	*with_slash;
	char	*full_path;
	char	**paths_array;

	if (!cmd || !shell)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (resolve_direct(cmd));
	paths_array = extract_path_to_array(shell);
	if (!paths_array)
		return (NULL);
	with_slash = ft_strjoin("/", cmd);
	full_path = find_in_paths(paths_array, with_slash);
	return (free(with_slash), free_array(paths_array), full_path);
}

// Exit code 127 is standard POSIX convention for "cmd not found"
// exit(127) occurs instead of returning from exec_child as
// it is a generic exit(EXIT_FAILURE) in fork_and_exec.
// FIX: added NULL guard to avoid NULL dereference
void	exec_child(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->args)
		return ;
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
