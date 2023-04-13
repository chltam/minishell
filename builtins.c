/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:28:19 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/09 19:34:37 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(char **args)
{
	int	n;

	n = 0;
	printf("exit\n");
	if (!args[1])
		return (0);
	while (args[1][n])
	{
		if (!ft_isdigit(args[1][n]))
		{
			printf("numeric argument required\n");
			return (2);
		}
		n++;
	}
	if (args[2])
		printf("too many arguments\n");
	return (ft_atoi(args[1]));
}

int	exec_builtin(t_cmd *cmd, t_envp *envl, bool *exit)
{
	int	exit_check;

	exit_check = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		exit_check = change_dir(cmd->args, envl);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		pwd();
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		exit_check = ft_env(envl);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		exit_check = ft_export(envl, cmd->args);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		exit_check = ft_unset(envl, cmd->args);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		exit_check = ft_exit(cmd->args);
		*exit = true;
	}
	return (exit_check);
}

int	is_builtin(char	*cmd)
{
	if (cmd)
	{
		if (ft_strcmp(cmd, "echo") == 0
			|| ft_strcmp(cmd, "cd") == 0
			|| ft_strcmp(cmd, "pwd") == 0
			|| ft_strcmp(cmd, "export") == 0
			|| ft_strcmp(cmd, "unset") == 0
			|| ft_strcmp(cmd, "env") == 0
			|| ft_strcmp(cmd, "exit") == 0)
			return (1);
	}
	return (0);
}
