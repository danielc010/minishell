#include "../include/minishell.h"

char **extract_path_to_array(t_shell *shell)
{
	int	i;
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

char *resolve_path(char *cmd, t_shell *shell)
{
	int	i;
	char *with_slash;
	char *full_path;
	char **paths_array;

	if (!cmd || !shell)
		return NULL;
	paths_array = extract_path_to_array(shell);
	if (!paths_array)
		return NULL;
	i = 0;
	with_slash = ft_strjoin("/", cmd);
	while (paths_array[i])
	{
		full_path = ft_strjoin(paths_array[i], with_slash);
		if ((access(full_path, X_OK)) == 0)
		{
			break ;
		}
		else
			free(full_path);
		++i;
	}
	if (paths_array[i] == NULL)
		return (free(with_slash), free_array(paths_array), NULL);
	return (free(with_slash), free_array(paths_array), full_path);
}

void fork_and_exec(t_cmd *cmd, t_shell *shell, t_pipe_ctx *ctx)
{
	pid_t	pid;
	int	status;

	if (!cmd || !shell)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		if (execve(resolve_path(cmd->args[0], shell), cmd->args, shell->env) == -1)
		{
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			shell->exit_status = WEXITSTATUS(status);
		}
	}
}
