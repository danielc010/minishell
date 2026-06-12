/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 18:40:46 by luimarti          #+#    #+#             */
/*   Updated: 2026/06/12 14:58:53 by luimarti         ###   ########.fr       */
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

void	init_lexer(char *line, t_lexer *lx)
{
	lx->line = line;
	lx->head = NULL;
	lx->tok = NULL;
	lx->state = NORMAL;
	lx->i = 0;
	lx->word_start = 0;
	lx->value = NULL;
	return ;
}

int	handle_normal(t_lexer *lx)
{
	if (lx->line[lx->i] == '|' || lx->line[lx->i] == '>'
		|| lx->line[lx->i] == '<')
	{
		if (!handle_operator(lx))
		{
			free_tokens(lx->head);
			return (0);
		}
	}
	else if (lx->line[lx->i] == '\'' || lx->line[lx->i] == '"')
		handle_quotes(lx);
	else if (lx->line[lx->i] != ' ' && lx->line[lx->i] != '\t')
	{
		lx->state = IN_WORD;
		lx->word_start = lx->i;
	}
	return (1);
}

t_token	*lexer(char *line)
{
	t_lexer	lx;

	if (!line)
		return (NULL);
	init_lexer(line, &lx);
	while (lx.line[lx.i])
	{
		if (lx.state == NORMAL)
		{
			if (!handle_normal(&lx))
				return (NULL);
		}
		else if (lx.state == IN_WORD)
		{
			if (!handle_word(&lx))
				return (NULL);
		}
		else if (lx.state == IN_SINGLE_QUOTE || lx.state == IN_DOUBLE_QUOTE)
			handle_quotes(&lx);
		lx.i++;
	}
	if (!handle_end_line(&lx))
		return (NULL);
	return (lx.head);
}
