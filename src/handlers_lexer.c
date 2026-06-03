/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handlers_lexer.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: luimarti <luimarti@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/04/21 14:18:01 by luimarti      #+#    #+#                 */
/*   Updated: 2026/06/03 15:26:35 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_end_line(t_lexer *lx)
{
	if (lx->state == IN_WORD)
	{
		lx->value = ft_substr(lx->line, lx->word_start, lx->i - lx->word_start);
		if (!lx->value)
		{
			free_tokens(lx->head);
			return (0);
		}
		lx->tok = new_token(WORD, lx->value);
		if (!lx->tok)
		{
			free_tokens(lx->head);
			free(lx->value);
			return (0);
		}
		add_token_back(&lx->head, lx->tok);
		free(lx->value);
	}
	else if (lx->state == IN_SINGLE_QUOTE || lx->state == IN_DOUBLE_QUOTE)
	{
		ft_putstr_fd("Syntax error: unclosed quote\n", 2);
		free_tokens(lx->head);
		return (0);
	}
	return (1);
}

int	handle_word(t_lexer *lx)
{
	if (is_separator(lx->line[lx->i]))
	{
		lx->value = ft_substr(lx->line, lx->word_start, lx->i - lx->word_start);
		if (!lx->value)
		{
			free_tokens(lx->head);
			return (0);
		}
		lx->tok = new_token(WORD, lx->value);
		if (!lx->tok)
		{
			free_tokens(lx->head);
			free(lx->value);
			return (0);
		}
		add_token_back(&lx->head, lx->tok);
		free(lx->value);
		lx->state = NORMAL;
		if (lx->line[lx->i] == '|' || lx->line[lx->i] == '<'
			|| lx->line[lx->i] == '>')
			lx->i--;
	}
	return (1);
}

void	handle_quotes(t_lexer *lx)
{
	if (lx->line[lx->i] == '\'' && lx->state == NORMAL)
	{
		lx->state = IN_SINGLE_QUOTE;
		lx->word_start = lx->i;
	}
	else if (lx->line[lx->i] == '"' && lx->state == NORMAL)
	{
		lx->state = IN_DOUBLE_QUOTE;
		lx->word_start = lx->i;
	}
	else if (lx->state == IN_SINGLE_QUOTE)
	{
		if (lx->line[lx->i] == '\'')
			lx->state = IN_WORD;
	}
	else if (lx->state == IN_DOUBLE_QUOTE)
	{
		if (lx->line[lx->i] == '"')
			lx->state = IN_WORD;
	}
	return ;
}

int	handle_double_operator(t_lexer *lx)
{
	if (lx->line[lx->i] == '>' && lx->line[lx->i + 1] == '>')
	{
		if (!add_operator_token(lx, REDIR_APPEND, ">>"))
			return (0);
		lx->i++;
	}
	else if (lx->line[lx->i] == '<' && lx->line[lx->i + 1] == '<')
	{
		if (!add_operator_token(lx, HEREDOC, "<<"))
			return (0);
		lx->i++;
	}
	return (1);
}

int	handle_operator(t_lexer *lx)
{
	if (lx->line[lx->i] == '|')
		return (add_operator_token(lx, PIPE, "|"));
	else if (lx->line[lx->i] == '>' && lx->line[lx->i + 1] == '>')
		return (handle_double_operator(lx));
	else if (lx->line[lx->i] == '<' && lx->line[lx->i + 1] == '<')
		return (handle_double_operator(lx));
	else if (lx->line[lx->i] == '>')
		return (add_operator_token(lx, REDIR_OUT, ">"));
	else if (lx->line[lx->i] == '<')
		return (add_operator_token(lx, REDIR_IN, "<"));
	return (1);
}
