/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interp_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:10:18 by htam              #+#    #+#             */
/*   Updated: 2023/04/03 20:13:45 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_env_var(t_lex *lex, t_envp *envp)
{
	while (lex)
	{
		if (lex->token == ENV_VAR)
		{
			lex->token = CMD_STR;
			lex->content = call_env_var(envp, lex->content);
		}
		lex = lex->next;
	}
}

void	check_dquote(t_lex *lex, t_envp *envp)
{
	t_lex	*minilex;

	while (lex)
	{
		minilex = NULL;
		if (lex->token == DQUOTE && *lex->content)
		{
			minilex = mini_lexer(lex->content, envp);
			node_merger(minilex);
			free(lex->content);
			lex->content = ft_strdup(minilex->content);
			ft_lstdelone_lex(minilex);
			lex->token = CMD_STR;
		}
		else if (lex->token == DQUOTE && !*lex->content)
			lex->token = CMD_STR;
		lex = lex->next;
	}
}

static int	is_redir(t_token token)
{
	if (token == REDIR_IN || token == REDIR_OUT
		|| token == APPEND || token == DELIMITER)
		return (1);
	else
		return (0);
}

void	check_redir(t_lex *lex)
{
	t_lex	*tmp;

	while (lex)
	{
		if (lex->next && is_redir(lex->token) && lex->next->token == CMD_STR)
		{
			lex->content = ft_strdup(lex->next->content);
			tmp = lex->next;
			lex->next = lex->next->next;
			ft_lstdelone_lex(tmp);
		}
		lex = lex->next;
	}
}

void	check_space(t_lex *lex)
{
	t_lex	*tmp;

	while (lex)
	{
		if (lex->next && lex->next->token == WHITE_SPACE)
		{
			tmp = lex->next;
			lex->next = lex->next->next;
			ft_lstdelone_lex(tmp);
		}
		lex = lex->next;
	}
}
