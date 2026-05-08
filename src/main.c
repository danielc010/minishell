#include "../include/minishell.h"

volatile sig_atomic_t	g_signal = 0; // volatile used so that the program reads this variable from memory instead of cache

int	main(int argc, char **argv, char **envp) // so we can access envp
{
	char *line;
	t_shell shell;

	(void)argc;
	(void)argv;
	dup_envp(&shell, envp);

	// temporary test — single command
	t_cmd cmd;
	char *args[] = {"/bin/ls", "-la", NULL}; // /bin/ls check
	// char *args[] = {"ls", "-la", NULL}; // normal command check
	cmd.args = args;
	cmd.redirs = NULL;
	cmd.next = NULL;
	shell.cmds = &cmd;
	fork_and_exec(&cmd, &shell);

	// temporary test — pipe: echo hello | cat
	t_cmd cmd2;
	char *args2[] = {"cat", NULL};
	cmd2.args = args2;
	cmd2.redirs = NULL;
	cmd2.next = NULL;

	t_cmd cmd1;
	char *args1[] = {"echo", "hello", NULL};
	cmd1.args = args1;
	cmd1.redirs = NULL;
	cmd1.next = &cmd2;

shell.cmds = &cmd1;
exec_pipeline(&shell);

	signals_interactive();
	rl_catch_signals = 0;
	while(1)
	{
		line = readline("shell> ");
		if (line == NULL) // EOF (ctrl-D)
		{
			if (g_signal == SIGINT)
			{
				g_signal = 0; // clear flag
				continue ; // loop reprints the prompt
			}
			break ; // exit out of the while loop
		}

		if (ft_strlen(line) > 0) //add_history as long as something was typed
			add_history(line);
		// fork_and_exec(cmd, &shell); // placeholder to insrt cmd from parser output
		free(line); // manual free as readline is designed as such
	}

	return (0);
}
