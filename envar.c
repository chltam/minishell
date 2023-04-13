/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:05:20 by htam              #+#    #+#             */
/*   Updated: 2023/04/03 22:31:16 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "minishell.h"

void	free_node(t_envp *envp)
{
	free(envp->name);
	free(envp->value);
	free(envp);
}

void	free_env_list(t_envp *envlist)
{
	t_envp	*temp;

	while (envlist)
	{
		temp = envlist->next;
		free(envlist->name);
		free(envlist->value);
		free(envlist);
		envlist = temp;
	}
}

void	add_node(t_envp **envlist, t_envp *node)
{
	t_envp	*last;

	if (!*envlist)
		*envlist = node;
	else
	{
		last = *envlist;
		while (last->next)
			last = last->next;
		last->next = node;
	}
}

char	**split_two(char *full)
{
	char	**str;
	int		start;
	int		n;

	str = malloc(sizeof(*str) * 2);
	start = 0;
	n = 0;
	while (full[start] == ' ')
		start++;
	while (full[start + n] && full[start + n] != '=')
		n++;
	str[0] = ft_substr(full + start, 0, n);
	n = start + n;
	while (full[n] && full[n] != '=')
		n++;
	if (full[n] != 0)
		str[1] = ft_substr(full + n + 1, 0, ft_strlen(full + n + 1));
	else
		str[1] = NULL;
	return (str);
}

void	ft_export(t_envp *envp, char *input)
{
	t_envp	*check;
	char	**str;
	t_envp	*node;

	check = envp;
	if (!input)
	{
		ft_env(envp);
		return ;
	}
	if (!ft_strchr(input, '='))
		return ;
	str = split_two(input);
	while (check)
	{	
		if (ft_strcmp(str[0], check->name) == 0)
		{
			free(check->value);
			check->value = str[1];
			free(str[0]);
			free(str);
			return ;
		}
		else
			check = check->next;
	}
	node = malloc(sizeof(*node));
	node->name = str[0];
	node->value = str[1];
	node->next = NULL;
	free(str);
	add_node(&envp, node);
}

void	ft_unset(t_envp *envp, char *name)
{
	t_envp	*temp;

	if (!name)
		return ;
	if (ft_strcmp(envp->name, name) == 0)
	{
		temp = envp->next;
		free_node(envp);
		*envp = *temp;
		return ;
	}
	while (envp->next && ft_strcmp(envp->next->name, name) != 0)
		envp = envp->next;
	if (envp->next && ft_strcmp(envp->next->name, name) == 0)
	{
		temp = envp->next;
		envp->next = temp->next;
		free_node(temp);
	}
}

t_envp	*get_env_list(char **envp)
{
	t_envp	*envlist;
	t_envp	*node;
	char	**str;

	envlist = NULL;
	while (*envp)
	{
		str = split_two(*envp);
		node = malloc(sizeof(*node));
		node->name = str[0];
		node->value = str[1];
		node->next = NULL;
		add_node(&envlist, node);
		free(str);
		envp++;
	}
	return (envlist);
}

void	ft_env(t_envp *envp)
{
	while (envp)
	{
		printf("%s=%s\n", envp->name, envp->value);
		envp = envp->next;
	}
}

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
	if (ft_strcmp(envl->name, name) == 0)
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

int	count_node(t_envp *envl)
{
	int	count;

	count = 0;
	while (envl)
	{
		count++;
		envl = envl->next;
	}
	return (count);
}

char	*node_to_str(t_envp *envl)
{
	char	*str;

	str = ft_strjoin(envl->name, "=");
	str = ft_strjoin_free_s1(str, envl->value);
	return (str);
}

char	**envl_to_envp(t_envp *envl)
{
	int		count;
	char	**envp;
	int		n;

	count = count_node(envl);
	envp = malloc(sizeof(*envp) * (count + 1));
	envp[count] = NULL;
	n = 0;
	if (!envp)
		return (NULL);
	while (envl)
	{
		envp[n] = node_to_str(envl);
		n++;
		envl = envl->next;
	}
	return (envp);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_envp	*envl;

// 	envl = get_env_list(envp);
// 	ft_export(envl, "test=123456");
// 	ft_export(envl, "test2=1234567");
// 	ft_export(envl, "tes=123456");
// 	ft_export(envl, "PATH=123456");
// 	// printf("before %p\n", envl);
// 	ft_unset(envl, "SYSTEMD_EXEC_PID");
// 	// printf("after %p\n", envl);
	
	
// 	free_env_list(envl);
	
// 	return (0);
// }
