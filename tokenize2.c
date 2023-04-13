#include "includes/minishell.h"

void	parse_delimiter(t_data *data, char *input, int tok_end)
{
	char	arg[100];
	int		i;

	i = -1;
	ft_memset(arg, '\0', 100);
	while (input[tok_end] && is_unused(input[tok_end]))
		tok_end++;
	if (!input[tok_end] || is_symbol(input[tok_end]))
	{
		printf("Error: wrong use of heredoc\n");
		return ;
	}
	while (input[tok_end] && !is_unused(input[tok_end]))
	{
		arg[++i] = input[tok_end];
		tok_end++;
	}
	data->delimiter = ft_strdup(arg);
}

/* void	parse_var(t_data *data, char *input, int *tok_end)
{
	t_list	*new;
	char	name[100];
	char	str[100];
	int		i;

	i = -1;
	ft_memset(name, '\0', 100);
	ft_memset(str, '\0', 100);
	if (!input[*tok_end] || is_symbol(input[*tok_end])
		|| is_unused(input[*tok_end]))
	{
		printf("\n");
		return ;
	}
	while (input[*tok_end] && input[*tok_end] != '=')
	{
		if (input[*tok_end] == ' ')
		{
			printf("\n");
			return ;
		}
		name[++i] = input[*tok_end];
		(*tok_end)++;
	}
	(*tok_end)++;
	if (!input[*tok_end])
	{
		printf("\n");
		return ;
	}
	i = -1;
	while (input[*tok_end] && !is_unused(input[*tok_end]))
	{
		str[++i] = input[*tok_end];
		(*tok_end)++;
	}
	new = ft_lstnew_var(ft_strdup(name), ft_strdup(str));
	ft_lstadd_back(&data->var_lst, new);
} */
