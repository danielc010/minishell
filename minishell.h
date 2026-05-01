/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 13:41:38 by luimarti          #+#    #+#             */
/*   Updated: 2026/05/01 17:46:27 by luimarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft/libft.h"

extern int	g_signal;

typedef enum e_state
{
	NORMAL,
	IN_WORD,
	IN_SINGLE_QUOTE,
	IN_DOUBLE_QUOTE,
}	t_state;
// Lexer state machine — tracks what context the lexer is currently in
// while scanning the input string character by character.
// NORMAL: outside any word or quote
// IN_WORD: inside a word (sequence of non-special characters)
// IN_SINGLE_QUOTE: inside single quotes — nothing is interpreted
// IN_DOUBLE_QUOTE: inside double quotes — only $ is interpreted

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

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_cmd			*cmds;
	char			**env;
	int				exit_status;
	int				running;
}	t_shell;

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

char	**copy_env(char **envp);
void	free_array(char **str);
void	free_cmd(t_cmd *commands);

//LEXER

int		add_operator_token(t_lexer *lx, t_token_type type, char *value);
int		is_separator(char c);
t_token	*new_token(t_token_type type, char *value);
void	add_token_back(t_token **head, t_token *new);
void	free_tokens(t_token *head);

int		handle_end_line(t_lexer *lx);
int		handle_word(t_lexer *lx);
void	handle_quotes(t_lexer *lx);
int		handle_double_operator(t_lexer *lx);
int		handle_operator(t_lexer *lx);

void	init_lexer(char *line, t_lexer *lx);
int		handle_normal(t_lexer *lx);
t_token	*lexer(char *line);

//PARSER

t_cmd	*init_cmd(void);
t_redir	*init_redir(void);
int		word_counter(t_token *tokens);
char	**build_args(t_token *tokens, int words);
t_cmd	*parsing_core(t_token *tokens);

t_redir	*build_redir(t_token *tokens);
int	fill_redir(t_token *tokens, t_redir **head, t_redir **current);
t_redir_type	redir_type(t_token *tokens);
int	add_redir(t_redir *new, t_token *tokens, t_redir **head, t_redir **current);
void	free_redir(t_redir *redirs);

#endif
