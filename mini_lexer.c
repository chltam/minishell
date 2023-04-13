/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 20:04:11 by htam              #+#    #+#             */
/*   Updated: 2023/04/09 18:24:33 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*mini_lexer_cmd(char *input, t_lex **minilex)
{
	t_lex	*node;

	node = ft_lstnew_lex(ft_strtok(input, "$"), CMD_STR);
	ft_lstadd_back_lex(minilex, node);
	while (*input && *input != '$')
		input++;
	return (input);
}

static char	*mini_lexer_env(char *input, t_lex **minilex)
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
	else if ((!ft_isalnum(*input) && *input != '_') || !*input)
		node = ft_lstnew_lex(ft_strdup("$"), CMD_STR);
	else
	{
		while (ft_isalnum(input[peek]) || input[peek] == '_')
			peek++;
		node = ft_lstnew_lex(ft_substr(input, 0, peek), ENV_VAR);
		input += peek;
	}
	ft_lstadd_back_lex(minilex, node);
	return (input);
}

t_lex	*mini_lexer(char *input, t_envp *envp)
{
	t_lex	*minilex;

	minilex = NULL;
	while (*input)
	{
		if (*input != '$')
			input = mini_lexer_cmd(input, &minilex);
		else if (*input == '$')
			input = mini_lexer_env(++input, &minilex);
	}
	check_env_var(minilex, envp);
	return (minilex);
}
