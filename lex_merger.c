/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_merger.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:03:32 by htam              #+#    #+#             */
/*   Updated: 2023/04/03 20:25:25 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_lex	*need_merge(t_lex *lex)
{
	while (lex)
	{
		if (lex->next && lex->token == CMD_STR && lex->next->token == CMD_STR)
			return (lex);
		lex = lex->next;
	}
	return (NULL);
}

void	node_merger(t_lex *lex)
{
	t_lex	*tmp;
	char	*new_content;

	lex = need_merge(lex);
	if (!lex)
		return ;
	else
	{
		new_content = ft_strjoin_free_s1(lex->content, lex->next->content);
		lex->content = new_content;
		tmp = lex->next->next;
		ft_lstdelone_lex(lex->next);
		lex->next = tmp;
		node_merger(lex);
	}
}
