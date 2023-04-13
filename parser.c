/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbadura <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:20:38 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/08 18:20:41 by cbadura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*parse_tokens(t_lex *head);
static t_cmd	**create_cmds(t_lex *head, int *size);
static void		parse_cmd_str(t_cmd *cmd, t_lex *tmp);
static void		parse_delim(t_lex *tmp, t_cmd *cmd);

t_cmd_arr	*parse_lex(t_lex *head)
{
	t_cmd_arr	*cmd_arr;
	t_cmd		**cmds;

	if (!head)
		return (NULL);
	cmd_arr = malloc(sizeof(t_cmd_arr));
	if (!cmd_arr)
		return (NULL);
	cmd_arr->size = get_cmds_size(head);
	cmds = create_cmds(head, &cmd_arr->size);
	if (!cmds)
		return (NULL);
	else
		cmd_arr->cmds = cmds;
	return (cmd_arr);
}

static t_cmd	**create_cmds(t_lex *head, int *size)
{
	t_cmd		**cmds;
	t_lex		*tmp;
	int			i;

	i = 0;
	cmds = malloc(sizeof(t_cmd) * *size);
	if (!cmds)
		return (NULL);
	tmp = head;
	while (tmp)
	{
		if (i < *size)
		{
			cmds[i] = parse_tokens(tmp);
			if (!cmds[i])
				return (NULL);
		}
		while (tmp && tmp->token != PIPE_TOK)
			tmp = tmp->next;
		if (tmp)
			tmp = tmp->next;
		i++;
	}
	return (cmds);
}

static t_cmd	*parse_tokens(t_lex *head)
{
	t_lex	*tmp;
	t_cmd	*cmd;
	int		check;

	check = 0;
	cmd = cmd_constructor();
	tmp = head;
	while (tmp)
	{
		if (!check)
		{
			parse_cmd_str(cmd, tmp);
			check = 1;
		}
		if (tmp->token == PIPE_TOK)
			break ;
		else if (tmp->token == DELIMITER)
			parse_delim(tmp, cmd);
		else
			parse_more(cmd, tmp);
		tmp = tmp->next;
	}
	return (cmd);
}

static void	parse_delim(t_lex *tmp, t_cmd *cmd)
{
	char	*heredoc;

	if (tmp->content)
	{
		if (cmd->heredoc == false)
		{
			cmd->delimiter = ft_strcat(cmd->delimiter, tmp->content);
			heredoc = read_heredoc(cmd->delimiter);
			if (!heredoc)
				return ;
			cmd->heredoc_txt = ft_strcat(cmd->heredoc_txt, heredoc);
		}
		else
		{
			free(cmd->delimiter);
			cmd->delimiter = ft_strdup(tmp->content);
			free(cmd->heredoc_txt);
			heredoc = read_heredoc(cmd->delimiter);
			if (!heredoc)
				return ;
			cmd->heredoc_txt = ft_strdup(heredoc);
		}
		free(heredoc);
		cmd->heredoc = true;
	}
}

static void	parse_cmd_str(t_cmd *cmd, t_lex *tmp)
{
	char	**args;

	args = ft_strtok_pro_max(tmp);
	cmd->args = args;
	cmd->cmd = args[0];
	cmd->arg = true;
}
