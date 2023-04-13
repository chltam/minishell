/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 21:46:35 by htam              #+#    #+#             */
/*   Updated: 2023/04/10 17:39:04 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_envp *envl)
{
	while (envl)
	{
		printf("%s=%s\n", envl->name, envl->value);
		envl = envl->next;
	}
	return (0);
}

static int	export_check(char **args)
{
	int	n;
	int	flag;

	n = 1;
	flag = 0;
	while (args[n])
	{
		if (!ft_isalpha(args[n][0]) && args[n][0] != '_')
		{
			printf("invalid input\n");
			flag = 1;
		}
		n++;
	}
	return (flag);
}

int	ft_export(t_envp *envl, char **args)
{
	char	**input;
	int		exit_check;

	exit_check = 0;
	if (!args[1])
		return (ft_env(envl));
	if (!ft_isalpha(args[1][0]) && args[1][0] != '_')
	{
		printf("invalid input\n");
		return (1);
	}
	if (!ft_strchr(args[1], '='))
		return (0);
	input = split_two(args[1]);
	ft_setenv(envl, input[0], input[1]);
	free_split_in(input);
	exit_check = export_check(args);
	return (exit_check);
}

void	unset_one(t_envp *envl, char *name)
{
	t_envp	*tmp;

	if (ft_strcmp(envl->name, name) == 0)
	{	
		free(envl->name);
		free(envl->value);
		envl->name = ft_strdup(envl->next->name);
		envl->value = ft_strdup(envl->next->value);
		tmp = envl->next;
		envl->next = envl->next->next;
		ft_lstdelone_envl(tmp);
		return ;
	}
	while (envl->next && ft_strcmp(envl->next->name, name) != 0)
		envl = envl->next;
	if (envl->next && ft_strcmp(envl->next->name, name) == 0)
	{
		tmp = envl->next;
		envl->next = envl->next->next;
		ft_lstdelone_envl(tmp);
	}
}

int	ft_unset(t_envp *envl, char **args)
{
	if (!args[1])
		return (0);
	args++;
	while (*args)
	{
		unset_one(envl, *args);
		args++;
	}
	return (0);
}
