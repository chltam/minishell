/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbadura <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:28:38 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/08 18:28:40 by cbadura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_pwd(void);
static void	cd_arg(t_envp *envl, char *current,
				char **args, int *exit_check);

int	change_dir(char **args, t_envp *envl)
{
	char	*dir;
	int		exit_check;
	char	*current;

	exit_check = 0;
	current = get_pwd();
	if (args[1] == NULL || args[1][0] == '~')
	{
		dir = ft_getenv(envl, "HOME");
		ft_setenv(envl, "OLDPWD", current);
		chdir(dir);
		ft_setenv(envl, "PWD", dir);
	}
	else if (args[1] && args[1][0] == '-')
	{
		dir = ft_getenv(envl, "OLDPWD");
		ft_setenv(envl, "PWD", dir);
		chdir(dir);
		ft_setenv(envl, "OLDPWD", current);
		pwd();
	}
	else
		cd_arg(envl, current, args, &exit_check);
	free(current);
	return (exit_check);
}

static void	cd_arg(t_envp *envl, char *current,
				char **args, int *exit_check)
{
	char	*new;

	*exit_check = chdir(args[1]);
	if (*exit_check != 0)
	{
		perror("cd failed");
		*exit_check = 1;
	}
	ft_setenv(envl, "OLDPWD", current);
	new = get_pwd();
	ft_setenv(envl, "PWD", new);
	free(new);
}

void	pwd(void)
{
	char	*dir;

	dir = malloc(100);
	ft_memset(dir, 0, 100);
	dir = getcwd(dir, 100);
	printf("%s\n", dir);
	free(dir);
}

static char	*get_pwd(void)
{
	char	*dir;

	dir = malloc(100);
	ft_memset(dir, 0, 100);
	dir = getcwd(dir, 100);
	return (dir);
}
