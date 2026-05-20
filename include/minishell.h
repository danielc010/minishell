#ifndef MINISHELL_H
# define MINISHELL_H

#include "../Libft/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
//readline

#include <stdlib.h>
// malloc, calloc, free

#include <signal.h>
// various signals

#include <unistd.h>
// write, fork, execve

#include <sys/wait.h>
// waitpid

#include <fcntl.h>


// extern declaration so all files can see it
// volatile ells the compiler "don't optimize reads of this away — it can change
// outside the normal flow of the program." Without it, the compiler might cache
// g_signal in a register and never re-read it from memory after the signal fires.
extern volatile sig_atomic_t	g_signal;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}	t_token_type;

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_token
{
	t_token_type	type;
	char	*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_redir_type	type;
	char			*file;
	int				fd;
	struct s_redir	*next;
}	t_redir;

// Struct containing information on the user input
typedef struct s_cmd
{
	char	**args;
	t_redir	*redirs;
	struct s_cmd	*next;
}	t_cmd;

// General information on the shell
// cmds: struct containing information on the user input
// env: cmds: struct containing information on the user input
// exit_status: stores the last command's exit code. if a command finished successfully (return 0) or not (return anything else)
// running: ? reserved as flag for main loop
typedef struct s_shell
{
	t_cmd	*cmds;
	char	**env;
	int		exit_status;
	int		running;
}	t_shell;

// signals.c
void	signals_interactive(void);

// utils.c
void	dup_envp(t_shell *shell, char **envp);
void	free_array(char **arr);
int		count_cmds(t_cmd *cmd);
int setup_heredoc(char *delim);

// cmd_execution.c
char	**extract_path_to_array(t_shell *shell);
char	*resolve_path(char *cmd, t_shell *shell);
void	fork_and_exec(t_cmd *cmd, t_shell *shell);
void	apply_redirs(t_redir *redir);

// pipes.c
void	exec_pipeline(t_shell *shell);
int		init_pipes(int **pipes, int n);
void	setup_child_fds(int **pipes, int i, int n);
void	close_all_pipes(int **pipes, int n);
void	free_pipes(int **pipes, int n);

#endif
