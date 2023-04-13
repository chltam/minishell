#include "minishell.h"

t_input	*get_input(char *raw_input)
{
	t_input	*input;
	int		start;
	int		n;

	start = 0;
	n = 0;
	if (!*raw_input)
		return (NULL);
	input = malloc(sizeof(*input));
	if (!input)
		return (NULL);
	while (raw_input[start] == ' ')
		start++;
	while (raw_input[start + n] && raw_input[start + n] != ' ')
		n++;
	input->cmd = ft_substr(raw_input, start, n);
	input->arg = ft_split(raw_input + start + n, ' ');
	return (input);
}

void	hub(char *raw_input, t_envp *envp)
{
	t_input	*input;

	input = get_input(raw_input);
	if (!input)
		return ;
	if (ft_strcmp(input->cmd, "env") == 0)
		ft_env(envp);
	if (ft_strcmp(input->cmd, "export") == 0)
		ft_export(envp, input->arg[0]);
	if (ft_strcmp(input->cmd, "unset") == 0)
		ft_unset(envp, input->arg[0]);

	free_split_in(input->arg);
	if (input->cmd)
		free(input->cmd);
	free(input);
}
