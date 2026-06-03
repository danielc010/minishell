/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucho <lucho@student.42.fr>                  +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/18 17:06:01 by lucho         #+#    #+#                 */
/*   Updated: 2026/06/03 15:26:58 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_pwd(void)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (path == NULL)
		return (1);
	printf("%s\n", path);
	free(path);
	return (0);
}
