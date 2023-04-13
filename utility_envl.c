/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_envl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 21:54:23 by htam              #+#    #+#             */
/*   Updated: 2023/04/05 22:25:15 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envp	*ft_lstnew_envl(char *name, char *value)
{
	t_envp			*head;

	head = NULL;
	head = malloc(sizeof(t_envp));
	if (!head)
		return (NULL);
	if (!name)
		head->name = NULL;
	else
		head->name = name;
	head->value = value;
	head->next = NULL;
	return (head);
}

void	ft_lstadd_back_envl(t_envp **lst, t_envp *new)
{
	t_envp	*tmp;

	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	tmp = ft_lstlast_envl(*lst);
	tmp->next = new;
}

t_envp	*ft_lstlast_envl(t_envp *lst)
{
	t_envp	*last;

	if (!lst)
		return (NULL);
	last = lst;
	while (last->next)
		last = last->next;
	return (last);
}

void	ft_lstclear_envl(t_envp **lst)
{
	t_envp	*temp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone_envl(*lst);
		*lst = temp;
	}
	lst = NULL;
}

void	ft_lstdelone_envl(t_envp *lst)
{
	if (!lst)
		return ;
	if (lst->name)
		free(lst->name);
	if (lst->value)
		free(lst->value);
	free(lst);
}
