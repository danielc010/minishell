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

void fork_and_exec(t_cmd *cmd, t_shell *shell)
{
	pid_t	*pids;
	int	num_args;
	int	i;
	int	j;

	if (!cmd || !shell)
		return ;
	i = 0;
	while (cmd->args[i])
		++i;
	pids = malloc(sizeof(pid_t) * i);
	j = 0;
	while (j < i)
	{
		pids[j] = fork();
		if (pids[j] == -1)
		{
			free_array(pids);
			return ;
		}
		if (pids[j] == 0)
		{
			if (execve(resolve_path(cmd->args[i], shell), cmd->args[i], shell->env[i]) ==- 1)
			{
				// blahblah
			}
		}
		++j;
	}
}
