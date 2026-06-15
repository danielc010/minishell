/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/03 10:56:58 by luimarti      #+#    #+#                 */
/*   Updated: 2026/06/15 16:21:11 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_str(char *str, t_shell *shell)
{
	char	*result;
	int		i;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i] != '\0')
		result = dispatcher(str, shell, result, &i);
	return (result);
}

static void	remove_empty_arg(t_cmd *cmds, char *tmp, int i)
{
	int	j;

	free(cmds->args[i]);
	j = i;
	while (cmds->args[j])
	{
		cmds->args[j] = cmds->args[j + 1];
		j++;
	}
	free(tmp);
}

// FIX: Add 2 null guards to avoid deference null
static void	expand_args(t_cmd *cmds, t_shell *shell)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!cmds->args)
		return ;
	while (cmds->args[i])
	{
		tmp = cmds->args[i];
		cmds->args[i] = get_str(tmp, shell);
		if (!cmds->args[i])
		{
			free(tmp);
			return ;
		}
		if (cmds->args[i][0] == '\0')
			remove_empty_arg(cmds, tmp, i);
		else
		{
			free(tmp);
			i++;
		}
	}
}

void	expander(t_cmd *cmds, t_shell *shell)
{
	t_redir	*redir;
	char	*tmp;

	while (cmds)
	{
		expand_args(cmds, shell);
		redir = cmds->redirs;
		while (redir)
		{
			tmp = redir->file;
			redir->file = get_str(tmp, shell);
			free(tmp);
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
