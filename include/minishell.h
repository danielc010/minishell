/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/29 10:32:25 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/03 15:25:10 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
// perror? currently not used
# include <stdio.h>

// readline(), rl_catch_signals, rl_on_new_line, rl_replace_line, rl_redisplay.
# include <readline/readline.h>
// provides add_history()
# include <readline/history.h>

// malloc, calloc, free, exit
# include <stdlib.h>

// Various Signals
// sigaction, SIGINT, SIGQUIT, SIG_DFL, SIG_IGN, and the sig_atomic_t type.
# include <signal.h>

// write, fork, execve, pipe
# include <unistd.h>

// waitpid related MACROS (WIFEXITED, WEXITSTATUS, WIFSIGNALED, WTERMSIG)
# include <sys/wait.h>

// open() and its flags: O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC, O_APPEND.
# include <fcntl.h>

// A signal can interrupt your program at any point — mid-assignment,
// mid-function, anywhere. If your signal handler writes to a variable while the
// main program is also reading it, you get a data race.
// extern declaration: so all files can see it
// volatile: tells the compiler "don't optimize reads of this away — it can
// change outside the normal flow of the program." Without it, the compiler
//  might cache g_signal in a register and never re-read it from memory
// after the signal fires.
// sig_atomic_t: integer type that the CPU guarantees can be read and written
// atomically — in a single uninterruptible operation. No partial reads, no torn
// writes. It's the only type C guarantees is safe to use in a signal handler.
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
	char			*value;
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
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

// General information on the shell
// cmds: struct containing information on the user input
// env: cmds: struct containing information on the user input
// exit_status: stores the last command's exit code. if a command finished
// successfully (return 0) or not (return anything else)
// running: ? reserved as flag for main loop
typedef struct s_shell
{
	t_cmd	*cmds;
	char	**env;
	int		exit_status;
	int		running;
}	t_shell;

typedef struct s_pipe_ctx
{
	int		**pipes;
	pid_t	*pids;
	int		n;
}	t_pipe_ctx;

// signals.c
void	signals_interactive(void);
void	signals_child(void);

// utils.c
void	free_partial_env(char **env, int count);
int		count_cmds(t_cmd *cmd);

// cmd_execution.c
char	**extract_path_to_array(t_shell *shell);
char	*find_in_paths(char **paths, char *with_slash);
char	*resolve_path(char *cmd, t_shell *shell);
void	exec_child(t_cmd *cmd, t_shell *shell);
void	fork_and_exec(t_cmd *cmd, t_shell *shell);

// cmd_execution2.d
int		open_redir_fd(t_redir *redir);
void	apply_redirs(t_redir *redir);
int		handle_heredoc_signal(void);
int		setup_heredoc(char *delim);

// pipes.c
void	prepare_heredocs(t_cmd *cmd);
void	wait_all(t_shell *shell, pid_t *pids, int n);
void	fork_children(t_shell *shell, t_cmd *cmd, t_pipe_ctx *ctx);
void	run_pipeline(t_shell *shell, t_cmd *cmd, int n);
void	exec_pipeline(t_shell *shell);

// pipes2.c
int		init_pipes(int **pipes, int n);
void	setup_child_fds(int **pipes, int i, int n);
void	close_all_pipes(int **pipes, int n);
void	free_pipes(int **pipes, int n);

// builtin.c
void	replace_env_entry(char **entry, char *newval);
void	add_env(t_shell *shell, int i, char *new_env);
void	update_env(t_shell *shell, char *key, char *value);
char	*get_env(t_shell *shell, char *key);
char	*get_cd_path(t_shell *shell, t_cmd *cmd);

// builtin2.c
void	builtin_cd(t_shell *shell, t_cmd *cmd);
void	builtin_exit(t_shell *shell, t_cmd *cmd);
int		key_index(t_shell *shell, char *key);
void	builtin_unset(t_shell *shell, t_cmd *cmd);

//builtin3.c
void	print_export(char *key, char *value);
void	no_arg_export(t_shell *shell);
void	builtin_export(t_shell *shell, t_cmd *cmd);

#endif
