#include "../include/minishell.h"

void	exec_pipeline(t_shell *shell)
{
	t_cmd	*cmd;
	int	n;
	int	i;
	int	**pipes;
	int pid;
	int status;
	pid_t	*pids;

	cmd = shell->cmds;
	n = count_cmds(cmd);
	if (n == 1)
	{
		fork_and_exec(shell->cmds, shell);
		return ;
	}
	pipes = malloc(sizeof(int *) * (n - 1));
	if (!pipes)
		return ;
	if (init_pipes(pipes, n - 1) == -1)
	{
		free_pipes(pipes, n - 1);
		return ;
	}
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
	{
		free_pipes(pipes, n - 1);
		return ;
	}
	// parent loop
	i = 0;
	while (i < n)
	{
		pid = fork();
		if (pid == -1)
			return ;
		if (pid == 0)
		{
				setup_child_fds(pipes, i , n);
				close_all_pipes(pipes, n - 1);
				if (execve(resolve_path(cmd->args[0], shell), cmd->args, shell->env) == -1)
					exit(EXIT_FAILURE);

		}
		else
		{
			pids[i] = pid;
			cmd = cmd->next;
			++i;
		}

	}
	close_all_pipes(pipes, n - 1);
	i = 0;
	while (i < n)
	{
		pid = pids[i];
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			shell->exit_status = WEXITSTATUS(status);
		}
		++i;
	}
}

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
