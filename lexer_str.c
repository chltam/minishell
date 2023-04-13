/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:30:09 by htam              #+#    #+#             */
/*   Updated: 2023/04/03 20:34:38 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*lexer_space(char *input, t_lex **lex)
{
	t_lex	*node;

	node = NULL;
	while (*input == ' ')
		input++;
	if (!ft_strchr("|><", *input) && *input)
		node = ft_lstnew_lex(ft_strdup(" "), WHITE_SPACE);
	ft_lstadd_back_lex(lex, node);
	return (input);
}

char	*lexer_cmd(char *input, t_lex **lex)
{
	t_lex	*node;

	node = ft_lstnew_lex(ft_strtok(input, " $\'\"|><"), CMD_STR);
	ft_lstadd_back_lex(lex, node);
	while (*input && !is_symbol(*input) && *input != ' ')
		input++;
	return (input);
}

char	*lexer_quote(char *input, t_lex **lex)
{
	t_lex	*node;
	int		peek_pos;

	node = NULL;
	peek_pos = 0;
	if (*input == '\'')
	{
		if (peek_quotes(++input, '\'', &peek_pos) == 1)
		{
			node = ft_lstnew_lex(ft_substr(input, 0, peek_pos), CMD_STR);
			input = input + peek_pos + 1;
		}
	}
	else if (*input == '\"')
	{
		if (peek_quotes(++input, '\"', &peek_pos) == 1)
		{	
			node = ft_lstnew_lex(ft_substr(input, 0, peek_pos), DQUOTE);
			input = input + peek_pos + 1;
		}
	}
	ft_lstadd_back_lex(lex, node);
	return (input);
}

char	*lexer_env(char *input, t_lex **lex)
{
	t_lex	*node;
	int		peek;

	node = NULL;
	peek = 0;
	if (*input == '?')
	{
		node = ft_lstnew_lex(ft_itoa(g_exit), CMD_STR);
		input++;
	}
	else if (*input == ' ' || !*input)
		node = ft_lstnew_lex(ft_strdup("$"), CMD_STR);
	else
	{
		while (ft_isalnum(input[peek]) || input[peek] == '_')
			peek++;
		node = ft_lstnew_lex(ft_substr(input, 0, peek), ENV_VAR);
		input += peek;
	}
	ft_lstadd_back_lex(lex, node);
	return (input);
}
