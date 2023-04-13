/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_lst.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbadura <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:27:01 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/08 18:27:03 by cbadura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lex	*ft_lstnew_lex(char *str, t_token token)
{
	t_lex			*head;

	head = NULL;
	head = malloc(sizeof(t_lex));
	if (!head)
		return (NULL);
	if (!str)
		head->content = NULL;
	else
		head->content = (void *)str;
	head->token = token;
	head->next = NULL;
	return (head);
}

void	ft_lstadd_back_lex(t_lex **lst, t_lex *new)
{
	t_lex	*tmp;

	if (!new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	tmp = ft_lstlast_lex(*lst);
	tmp->next = new;
}

t_lex	*ft_lstlast_lex(t_lex *lst)
{
	t_lex	*last;

	if (!lst)
		return (NULL);
	last = lst;
	while (last->next)
		last = last->next;
	return (last);
}

void	ft_lstclear_lex(t_lex **lst)
{
	t_lex	*temp;

	if (!lst || !(*lst))
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		ft_lstdelone_lex(*lst);
		*lst = temp;
	}
	lst = NULL;
}

void	ft_lstdelone_lex(t_lex *lst)
{
	if (!lst)
		return ;
	if (lst->content)
		free(lst->content);
	free(lst);
}
