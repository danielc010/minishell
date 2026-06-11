/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: luimarti <luimarti@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/29 10:32:25 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/11 15:53:12 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

// perror
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

// opendir, readdir, closedir — directory traversal functions
# include <dirent.h>

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

typedef enum e_state
{
	NORMAL,
	IN_WORD,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE,
}	t_state;

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

typedef struct s_lexer
{
	char	*line;
	t_token	*head;
	t_token	*tok;
	t_state	state;
	int		i;
	int		word_start;
	char	*value;
}	t_lexer;

// signals.c
void			signals_interactive(void);
void			signals_waiting(void);
void			signals_child(void);

// utils.c
void			free_partial_env(char **env, int count);
void			dup_envp(t_shell *shell, char **envp);
void			free_array(char **arr);
int				count_cmds(t_cmd *cmd);

// cmd_execution.c
char			**extract_path_to_array(t_shell *shell);
char			*find_in_paths(char **paths, char *with_slash);
char			*resolve_path(char *cmd, t_shell *shell);
void			exec_child(t_cmd *cmd, t_shell *shell);

// cmd_execution2.c
void			fork_and_exec(t_cmd *cmd, t_shell *shell);
int				open_redir_fd(t_redir *redir);
void			apply_redirs(t_redir *redir);
int				handle_heredoc_signal(void);
int				setup_heredoc(char *delim);

// pipes.c
void			prepare_heredocs(t_cmd *cmd);
void			wait_all(t_shell *shell, pid_t *pids, int n);
void			child_state_builtins(t_shell *shell, t_cmd *cmd);
void			run_builtin_in_child(t_shell *shell, t_cmd *cmd);
void			fork_children(t_shell *shell, t_cmd *cmd, t_pipe_ctx *ctx);

// pipes2.c
void			run_pipeline(t_shell *shell, t_cmd *cmd, int n);
void			exec_pipeline(t_shell *shell);
int				init_pipes(int **pipes, int n);

// pipes3.c
void			setup_child_fds(int **pipes, int i, int n);
void			close_all_pipes(int **pipes, int n);
void			free_pipes(int **pipes, int n);

// builtin_env_utils.c
void			replace_env_entry(char **entry, char *newval);
void			add_env(t_shell *shell, int i, char *new_env);
void			update_env(t_shell *shell, char *key, char *value);
char			*get_env(t_shell *shell, char *key);
char			*get_cd_path(t_shell *shell, t_cmd *cmd);

// builtin_cd_exit.c
void			builtin_cd(t_shell *shell, t_cmd *cmd);
void			builtin_exit(t_shell *shell, t_cmd *cmd);

// builtin_unset.c
int				key_index(t_shell *shell, char *key);
void			remove_env_entry(t_shell *shell, char *key);
void			builtin_unset(t_shell *shell, t_cmd *cmd);

// builtin_export.c
void			no_arg_export(t_shell *shell);
void			export_key_value(t_shell *shell, char *arg, char *eq);
void			export_no_value(t_shell *shell, char *arg);
void			builtin_export(t_shell *shell, t_cmd *cmd);

// builtin_export_utils.c
void			print_export(char *key, char *value);
int				is_valid_identifier(char *str);
void			exist_check(t_shell *shell, char *key);
void			invalid_identifier(t_shell *shell, char *arg, char *builtin);

// utils_parser.c
void			free_cmd(t_cmd *commands);

//LEXER
int				add_operator_token(t_lexer *lx, t_token_type type, char *value);
int				is_separator(char c);
t_token			*new_token(t_token_type type, char *value);
void			add_token_back(t_token **head, t_token *new);
void			free_tokens(t_token *head);

int				handle_end_line(t_lexer *lx);
int				handle_word(t_lexer *lx);
void			handle_quotes(t_lexer *lx);
int				handle_double_operator(t_lexer *lx);
int				handle_operator(t_lexer *lx);

void			init_lexer(char *line, t_lexer *lx);
int				handle_normal(t_lexer *lx);
t_token			*lexer(char *line);

//PARSER

t_cmd			*init_cmd(void);
t_redir			*init_redir(void);
int				word_counter(t_token *tokens);
char			**build_args(t_token *tokens, int words);
t_cmd			*parsing_core(t_token *tokens);

t_redir			*build_redir(t_token *tokens);
int				fill_redir(t_token *tokens, t_redir **head, t_redir **current);
t_redir_type	redir_type(t_token *tokens);
int				add_redir(t_redir *new, t_token *tokens, t_redir **head,
					t_redir **current);
void			free_redir(t_redir *redirs);

int				syntax_controller(t_token *tokens);
int				check_redirs(t_token *tokens);
int				check_pipes(t_token *tokens);

//EXPANDER

void			expander(t_cmd *cmds, t_shell *shell);
char			*find_value(char *name, t_shell *shell);
char			*get_var_name(char *str);
char			*append_char(char *str, char *result, int *i);
char			*expand_var(char *str, t_shell *shell, char *result, int *i);
char			*single_quote_handler(char *str, char *result, int *i);
char			*double_quote_handler(char *str, char *result, t_shell *shell,
					int *i);
char			*special_handler(char *result, t_shell *shell, int *i);
char			*dispatcher(char *str, t_shell *shell, char *result, int *i);
char			*get_str(char *str, t_shell *shell);
char			*dollar_handler(char *str, char *result, t_shell *shell,
					int *i);

// builtin_echo.c
int				is_n_flag(char *str);
int				ft_echo(char **args);

// builtin_env.c
char			**copy_env(char **envp);
int				ft_env(t_shell *shell);

// builtin_pwd.c
int				ft_pwd(void);

//CORE

t_cmd			*parse(char *line, t_shell *shell);
void			init_shell(t_shell *shell, char **envp);

#endif
