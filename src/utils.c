#include "../include/minishell.h"

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
