#include "minishell.h"

static char	get_token(char *input, int *pos, int *tok_st, int *tok_end);
static char	parse_token(char *input, int *pos, char token);
static void	parse_options(t_data *data, char *input, char token, int tok_end);
static void	parse_redir(t_data *data, char *input, char token, int tok_end);

void	tokenize(t_data *data, char *input)
{
	char	token;
	char	token_peek;
	int		pos;
	int		tok_st;
	int		tok_end;

	pos = 0;
	while (input[pos])
	{
		while (input[pos] && is_unused(input[pos]))
			pos++;
		token_peek = peek(input, pos);
		token = get_token(input, &pos, &tok_st, &tok_end);
		printf("token: '%c'\n", token);
		parse_options(data, input, token, tok_end);
	}
}

static char	get_token(char *input, int *pos, int *tok_st, int *tok_end)
{
	char	token;

	while (input[*pos] && !is_symbol(input[*pos]))
		(*pos)++;
	if (tok_st)
		*tok_st = *pos;
	token = input[*pos];
	if (!input[*pos])
		return (input[*pos]);
	token = parse_token(input, pos, token);
	if (*tok_end)
		*tok_end = *pos;
	while (input[*pos] && is_unused(input[*pos]))
		(*pos)++;
	return (token);
}

static char	parse_token(char *input, int *pos, char token)
{
	/* if (is_monosymbol(input[*pos]))
		(*pos)++; */
	if (input[*pos] == '<')
	{
		(*pos)++;
		if (input[*pos] == '<')
		{
			token = '@';
			(*pos)++;
		}
	}
	else if (input[*pos] == '>')
	{
		(*pos)++;
		if (input[*pos] == '>')
		{
			token = '+';
			(*pos)++;
		}
	}
	return (token);
}

static void	parse_options(t_data *data, char *input, char token, int tok_end)
{
	if (token == '<' || token == '>')
		parse_redir(data, input, token, tok_end);
	if (token == '@')
		parse_delimiter(data, input, tok_end);
}

static void	parse_redir(t_data *data, char *input, char token, int tok_end)
{
	char	arg[100];
	int		i;

	i = -1;
	ft_memset(arg, '\0', 100);
	while (input[tok_end] && is_unused(input[tok_end]))
		tok_end++;
	if (!input[tok_end] || is_symbol(input[tok_end]))
	{
		printf("Error: wrong use of redirection\n");
		return ;
	}
	while (input[tok_end] && !is_unused(input[tok_end]))
	{
		arg[++i] = input[tok_end];
		tok_end++;
	}
	if (token == '<')
		data->in_file = ft_strdup(arg);
	else if (token == '>')
		data->out_file = ft_strdup(arg);
}
