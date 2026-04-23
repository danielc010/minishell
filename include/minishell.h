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


// extern declaration so all files can see it
extern int	g_signal;

typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    HEREDOC
}   t_token_type;

typedef enum e_redir_type
{
    R_IN,
    R_OUT,
    R_APPEND,
    R_HEREDOC
}   t_redir_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct s_redir
{
    t_redir_type    type;
    char            *file;
    int             fd;
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char            **args;
    t_redir         *redirs;
    struct s_cmd    *next;
}   t_cmd;

typedef struct s_shell
{
    t_cmd           *cmds;
    char            **env;
    int             exit_status;
    int             running;
}   t_shell;

// signals.c
void	signals_interactive(void);

// utils.c
void	dup_envp(t_shell *shell, char **envp);
void	free_array(char **arr);

//cmd_execution.c
char **extract_path_to_array(t_shell *shell);
char *resolve_path(char *cmd, t_shell *shell);
void fork_and_exec(t_cmd *cmd, t_shell *shell);

#endif
