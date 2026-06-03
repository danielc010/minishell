/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cmd_execution.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/20 23:39:14 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/03 16:10:51 by dachin        ########   odam.nl         */
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

// if ((access(full_path, X_OK)) == 0): path exists and is executable so
// skip to the last return statement
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

	if (!cmd || !shell)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
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

// Exit code 127 is standard POSIX convention for "cmd not found"
// exit(127) occurs instead of returning from exec_child as
// it is a generic exit(EXIT_FAILURE) in fork_and_exec.
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

// When a child process finishes, it doesnt disapppear. It stays in the
// process as a zombie proesss. waiting for its parent to collect
// its exit status with waitpid for clean up, otherwise there will be
// process table leak where no new processes can be created.
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
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
		signals_interactive();
	}
}
