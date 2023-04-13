/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 21:37:58 by htam              #+#    #+#             */
/*   Updated: 2023/04/10 20:28:23 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_lex(t_lex *lex)
// {
// 	while (lex)
// 	{
// 		printf("-------------\n");
// 		switch (lex->token)
// 		{
// 			case 0:
// 				printf("token = CMD_STR\n");
// 			break ;
// 			case 1:
// 				printf("token = DQUOTE\n");
// 			break ;
// 			case 2:
// 				printf("token = SQUOTE\n");
// 			break ;
// 			case 3:
// 				printf("token = REDIR_IN\n");
// 			break ;
// 			case 4:
// 				printf("token = REDIR_OUT\n");
// 			break ;
// 			case 5:
// 				printf("token = DELIMITER\n");
// 			break ;
// 			case 6:
// 				printf("token = APPEND\n");
// 			break ;
// 			case 7:
// 				printf("token = ENV_VAR\n");
// 			break ;
// 			case 8:
// 				printf("token = PIPE_TOK\n");
// 			break ;
// 			case 9:
// 				printf("token = WHITE_SPACE\n");
// 			break ;
// 		}
// 		printf("content = %s\n", lex->content);
// 		if (!lex->next)
// 			printf("-------------\n");
// 		lex = lex->next;
// 	}
// }

void	interpreter(t_lex *lex, t_envp *envp)
{
	check_env_var(lex, envp);
	check_dquote(lex, envp);
	node_merger(lex);
	check_redir(lex);
	check_space(lex);
}

static int	args_count(t_lex *lex)
{
	int	count;

	count = 0;
	while (lex && lex->token != PIPE_TOK)
	{
		if (lex->token == CMD_STR)
			count++;
		lex = lex->next;
	}
	return (count);
}

char	**ft_strtok_pro_max(t_lex *lex)
{
	int				count;
	char			**args;
	int				n;

	if (lex == NULL)
		return (NULL);
	count = args_count(lex);
	n = 0;
	args = malloc(sizeof(*args) * (count + 1));
	if (!args)
		return (NULL);
	args[count] = NULL;
	while (lex && lex->token != PIPE_TOK)
	{
		if (lex->token == CMD_STR)
		{
			args[n] = ft_strdup(lex->content);
			n++;
		}
		lex = lex->next;
	}
	if (lex && lex->token == PIPE_TOK)
		lex = lex->next;
	return (args);
}

t_lex	*ft_lexer(char *input, t_envp *envp)
{
	t_lex	*lex;

	lex = NULL;
	while (*input == ' ')
		input++;
	while (*input)
	{
		if (*input == ' ')
			input = lexer_space(input, &lex);
		else if (*input == '|')
			input = lexer_pipe(input, &lex);
		else if (!is_symbol(*input))
			input = lexer_cmd(input, &lex);
		else if (*input == '\'' || *input == '\"')
			input = lexer_quote(input, &lex);
		else if (*input == '$')
			input = lexer_env(++input, &lex);
		else if (*input == '>')
			input = lexer_out(++input, &lex);
		else if (*input == '<')
			input = lexer_in(++input, &lex);
	}
	interpreter(lex, envp);
	return (lex);
}
