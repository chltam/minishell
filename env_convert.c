/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 22:32:08 by htam              #+#    #+#             */
/*   Updated: 2023/04/10 20:27:21 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**split_two(char *full)
{
	char	**str;
	int		start;
	int		n;

	str = malloc(sizeof(*str) * 3);
	if (!str)
		return (NULL);
	str[2] = NULL;
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

t_envp	*get_env_list(char **envp)
{
	t_envp	*envlist;
	t_envp	*node;
	char	**str;

	envlist = NULL;
	while (*envp)
	{
		str = split_two(*envp);
		node = ft_lstnew_envl(str[0], str[1]);
		ft_lstadd_back_envl(&envlist, node);
		free(str);
		envp++;
	}
	return (envlist);
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
