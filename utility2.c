/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:20:03 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/09 20:36:52 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_input(char *input)
{
	char	**split;

	split = ft_split(input, ' ');
	if (!split)
		return (NULL);
	return (split);
}

void	free_split_in(char **split_in)
{
	int	i;

	i = 0;
	if (!split_in)
		return ;
	while (split_in[i])
	{
		free(split_in[i]);
		i++;
	}
	free(split_in);
}

void	del(void *content)
{
	free(content);
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->in_file)
		free(cmd->in_file);
	if (cmd->out_file)
		free(cmd->out_file);
	if (cmd->delimiter)
		free(cmd->delimiter);
	if (cmd->heredoc_txt)
		free(cmd->heredoc_txt);
	if (cmd->arg == true)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}

void	free_cmd_arr(t_cmd_arr *cmd_arr)
{
	int	i;

	if (!cmd_arr)
		return ;
	i = 0;
	while (i < cmd_arr->size)
	{
		if (cmd_arr->cmds[i])
			free_cmd(cmd_arr->cmds[i]);
		i++;
	}
	free(cmd_arr->cmds);
	free(cmd_arr);
}
