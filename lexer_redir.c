/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:26:35 by htam              #+#    #+#             */
/*   Updated: 2023/04/03 20:32:03 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*lexer_pipe(char *input, t_lex **lex)
{
	t_lex	*node;

	node = ft_lstnew_lex(ft_strdup("|"), PIPE_TOK);
	ft_lstadd_back_lex(lex, node);
	input++;
	while (*input == ' ')
		input++;
	return (input);
}

char	*lexer_out(char *input, t_lex **lex)
{
	t_lex	*node;

	if (*input == '>')
	{
		node = ft_lstnew_lex(NULL, APPEND);
		input++;
	}
	else
		node = ft_lstnew_lex(NULL, REDIR_OUT);
	ft_lstadd_back_lex(lex, node);
	while (*input == ' ')
		input++;
	return (input);
}

char	*lexer_in(char *input, t_lex **lex)
{
	t_lex	*node;

	if (*input == '<')
	{
		node = ft_lstnew_lex(NULL, DELIMITER);
		input++;
	}
	else
		node = ft_lstnew_lex(NULL, REDIR_IN);
	ft_lstadd_back_lex(lex, node);
	while (*input == ' ')
		input++;
	return (input);
}
