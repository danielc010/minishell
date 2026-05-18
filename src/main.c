#include "../include/minishell.h"

int	g_signal = 0;

int	main(int argc, char **argv, char **envp) // so we can access envp
{
	char	*line;
	t_shell	shell;
	t_cmd	cmd1;
	// t_cmd	cmd2;

	(void)argc;
	(void)argv;
	dup_envp(&shell, envp);
	signals_interactive();
	char *cmd1_args[] = {"echo", "world", NULL};
	cmd1.args = cmd1_args;
	cmd1.next = NULL;
	// char *cmd2_args[] = {"cat", NULL};
	// cmd2.args = cmd2_args;
	// cmd1.next = &cmd2;
	// cmd2.next = NULL;
	shell.cmds = &cmd1;

	t_redir redir1;
	redir1.type = R_APPEND;
	redir1.file = "/tmp/test.txt";
	redir1.fd = 0;
	redir1.next = NULL;
	cmd1.redirs = &redir1;


	while(1)
	{
		line = readline("shell> ");
		if (line == NULL) // EOF (ctrl-D)
		{
			break ;
		}
		if (g_signal == SIGINT)
		{
			write(1, "\n", 1);
			g_signal = 0;
			free(line);
			continue ;
		}
		if (ft_strlen(line) > 0) //add_history as long as something was typed
			add_history(line);
		exec_pipeline(&shell); // placeholder to insert cmd from parser output
		free(line); // manual free as readline is designed as such
	}

	return (0);
}
