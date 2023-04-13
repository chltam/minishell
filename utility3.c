/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:20:18 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/10 19:54:27 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	peek(char *input, int peek_pos)
{
	while (input[peek_pos])
	{
		if (is_symbol(input[peek_pos]))
			return (input[peek_pos]);
		peek_pos++;
	}
	return ('\0');
}

int	peek_quotes(char *input, char quote, int *peek_pos)
{
	while (input[*peek_pos])
	{
		if (input[*peek_pos] == quote)
			return (1);
		(*peek_pos)++;
	}
	return (0);
}

void	check_infile(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1)
		return ;
	close(fd);
}

int	is_symbol(char c)
{
	int	i;

	i = 0;
	while (SYMBOLS[i])
	{
		if (c == SYMBOLS[i])
			return (1);
		i++;
	}
	return (0);
}

void	child_is_builtin(t_cmd *cmd, t_envp *envl)
{
	int	exit_check;

	exit_check = 0;
	exit_check = exec_builtin(cmd, envl, false);
	if (!exit_check)
		exit(EXIT_SUCCESS);
	else
		exit(EXIT_FAILURE);
}
