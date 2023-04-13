/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:20:53 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/10 15:31:54 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_redir_in(t_lex *tmp, t_cmd *cmd);
static void	parse_redir_out(t_lex *tmp, t_cmd *cmd);

void	parse_more(t_cmd *cmd, t_lex *tmp)
{
	int	fd_out;

	if (tmp->token == REDIR_IN)
		parse_redir_in(tmp, cmd);
	else if (tmp->token == REDIR_OUT)
		parse_redir_out(tmp, cmd);
	else if (tmp->token == APPEND)
	{
		if (tmp->content)
		{
			fd_out = open(tmp->content, O_CREAT | O_RDWR, 0644);
			if (fd_out == -1)
				perror("File could not be created");
			close(fd_out);
			if (cmd->append == false && cmd->redir_out == false)
				cmd->out_file = ft_strcat(cmd->out_file, tmp->content);
			else
			{
				free(cmd->out_file);
				cmd->out_file = ft_strdup(tmp->content);
			}
			cmd->append = true;
		}
	}
}

static void	parse_redir_in(t_lex *tmp, t_cmd *cmd)
{
	if (tmp->content)
	{
		if (cmd->redir_in == false)
		{
			check_infile(tmp->content);
			cmd->in_file = ft_strcat(cmd->in_file, tmp->content);
		}
		else
		{
			free(cmd->in_file);
			cmd->in_file = ft_strdup(tmp->content);
		}
	}
	cmd->redir_in = true;
}

static void	parse_redir_out(t_lex *tmp, t_cmd *cmd)
{
	int	fd_out;

	if (tmp->token == REDIR_OUT)
	{
		if (tmp->content)
		{
			fd_out = open(tmp->content, O_CREAT | O_RDWR, 0644);
			if (fd_out == -1)
				perror("File could not be created");
			close(fd_out);
			if (cmd->redir_out == false && cmd->append == false)
				cmd->out_file = ft_strcat(cmd->out_file, tmp->content);
			else
			{
				free(cmd->out_file);
				cmd->out_file = ft_strdup(tmp->content);
				cmd->append = false;
			}
		}
		cmd->redir_out = true;
	}
}

int	get_cmds_size(t_lex *head)
{
	t_lex	*tmp;
	int		size;

	size = 1;
	tmp = head;
	while (tmp)
	{
		if (tmp->token == PIPE_TOK && tmp->next)
			size++;
		tmp = tmp->next;
	}
	return (size);
}

char	*read_heredoc(char *delimiter)
{
	char	*here;
	char	*buffer;

	here = malloc(1000);
	if (!here)
		return (NULL);
	ft_memset(here, 0, 1000);
	while (1)
	{
		write(1, "heredoc> ", 9);
		buffer = get_next_line(0);
		if (!buffer)
		{
			quit_heredoc(here, delimiter);
			return (NULL);
		}
		if (buffer && ft_strncmp(buffer, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(buffer);
			break ;
		}
		else
			fill_heredoc(here, buffer);
	}
	return (here);
}
