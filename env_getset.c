/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_getset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:05:20 by htam              #+#    #+#             */
/*   Updated: 2023/04/05 22:37:20 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "minishell.h"

char	*ft_getenv(t_envp *envl, char *name)
{
	if (!envl || !name)
		return (NULL);
	while (envl && ft_strcmp(envl->name, name) != 0)
		envl = envl->next;
	if (envl && ft_strcmp(envl->name, name) == 0)
		return (envl->value);
	else
		return (NULL);
}

void	ft_setenv(t_envp *envl, char *name, char *value)
{
	t_envp	*head;
	t_envp	*node;

	if (!envl || !name || !value)
		return ;
	head = envl;
	while (envl && ft_strcmp(envl->name, name) != 0)
		envl = envl->next;
	if (envl && ft_strcmp(envl->name, name) == 0)
	{
		free(envl->value);
		envl->value = ft_strdup(value);
	}
	else if (!envl)
	{
		node = ft_lstnew_envl(ft_strdup(name), ft_strdup(value));
		ft_lstadd_back_envl(&head, node);
	}
	return ;
}

char	*call_env_var(t_envp *envl, char *name)
{
	if (!envl || !name)
		return (NULL);
	while (envl && ft_strcmp(envl->name, name) != 0)
		envl = envl->next;
	if (envl && ft_strcmp(envl->name, name) == 0)
	{
		free(name);
		return (ft_strdup(envl->value));
	}
	else
	{
		free(name);
		return (ft_strdup(""));
	}
}
