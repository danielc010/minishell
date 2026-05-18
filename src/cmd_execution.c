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
	if (cmd[0] == '/')
		return cmd;
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
	pid_t	pid;
	int	status;
	char *temp;

	if (!cmd || !shell)
		return ;
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		temp = resolve_path(cmd->args[0], shell);
		apply_redirs(cmd->redirs); // positioned here because everything after successful execve will not run
		if (execve(temp, cmd->args, shell->env) == -1)
		{
			free(temp); // TODO: To fix this free later. The absolute path case in resolve_path returns cmd directly (not malloc'd), so free(temp) would be wrong here.
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

void apply_redirs(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == R_IN)
		{
			fd = open(redir->file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == R_OUT)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->type == R_APPEND)
		{
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
}
