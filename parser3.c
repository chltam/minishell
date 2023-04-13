/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:21:09 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/10 15:24:42 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quit_heredoc(char *here, char *delimiter)
{
	printf("here-doc delimited by EOF, wanted '%s'. ", delimiter);
	printf("Press CTRL-D again!\n");
	free(here);
}

void	fill_heredoc(char *here, char *buffer)
{
	here = ft_strcat(here, buffer);
	here = ft_strcat(here, "\n");
	free(buffer);
}

void	init_cmd_redir(t_cmd *cmd)
{
	cmd->redir_in = false;
	cmd->redir_out = false;
	cmd->heredoc = false;
	cmd->append = false;
}

t_cmd	*cmd_constructor(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, '\0', sizeof(cmd));
	cmd->in_file = malloc(100);
	if (!cmd->in_file)
		return (NULL);
	ft_memset(cmd->in_file, '\0', 100);
	cmd->out_file = malloc(100);
	if (!cmd->out_file)
		return (NULL);
	ft_memset(cmd->out_file, '\0', 100);
	cmd->delimiter = malloc(100);
	if (!cmd->delimiter)
		return (NULL);
	ft_memset(cmd->delimiter, '\0', 100);
	cmd->heredoc_txt = malloc(100);
	if (!cmd->heredoc_txt)
		return (NULL);
	ft_memset(cmd->heredoc_txt, '\0', 100);
	init_cmd_redir(cmd);
	return (cmd);
}
