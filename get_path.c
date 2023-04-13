/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:31:18 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/09 21:14:41 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(char **allpath, char **temp_envp)
{
	free_split_in(allpath);
	free_split_in(temp_envp);
	ft_error("No such command");
}

char	*valid_path(char **allpath, char *cmd)
{
	char	*tempcmd;
	char	*path;

	tempcmd = ft_strjoin("/", cmd);
	while (*allpath)
	{
		path = ft_strjoin(*allpath, tempcmd);
		if (access(path, F_OK) == 0)
		{
			free(tempcmd);
			return (path);
		}
		else
		{
			free(path);
			allpath++;
		}
	}
	free(tempcmd);
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	**allpath;
	char	*path;
	char	**temp_envp;

	temp_envp = envp;
	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	if (!*envp)
	{
		free_split_in(temp_envp);
		ft_error("No such command");
		return (NULL);
	}
	allpath = ft_split(*envp + 5, ':');
	path = valid_path(allpath, cmd);
	if (path)
	{
		free_split_in(allpath);
		return (path);
	}
	else
		free_and_exit(allpath, temp_envp);
	return (NULL);
}
