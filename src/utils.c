#include "../include/minishell.h"

// We copy because envp is owned by the OS/runtime — you can't modify it. But your shell needs to modify the environment when the user runs export or unset. Those built-ins add, change, or remove variables.
// If you used envp directly, those changes would be impossible. With your own copy in shell.env, you can freely modify it.
void	dup_envp(t_shell *shell, char **envp)
{
	int	i;
	char **envp_cpy;
	int j;

	if (!shell || !envp)
		return ;
	i = 0;
	j = 0;
	while (envp[i])
		++i;
	envp_cpy = malloc(sizeof(char *) * (i + 1));
	if (!envp_cpy)
		return ;
	while (envp[j])
	{
		envp_cpy[j] = ft_strdup(envp[j]);
		if (!envp_cpy[j])
		{
			while (j < i)
			{
				free(envp_cpy[j]);
				++j;
			}
			free(envp_cpy);
			return ;
		}
		++j;
	}
	envp_cpy[j] = NULL;
	shell->env = envp_cpy;
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		++i;
	}
	free(arr);
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

int	count_cmds(t_cmd *cmd)
{
	int i;

	if (!cmd)
		return (0);
	i = 1;
	while (cmd->next)
	{
		cmd = cmd->next;
		++i;
	}
	return i;
}
