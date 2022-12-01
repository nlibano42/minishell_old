/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlibano- <nlibano-@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 01:14:15 by nlibano-          #+#    #+#             */
/*   Updated: 2022/12/01 11:24:12 by nlibano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_lstnew(char *name, char *val)
{
	t_stack	*list;

	list = malloc(sizeof(t_env));
	if (!list)
		return (NULL);
	list->name = name;
	list->val = val;
	list->next = NULL;
	return (list);
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*last_lst;

	if (!*lst)
		*lst = new;
	else
	{
		last_lst = ft_lstlast(*lst);
		last_lst->next = new;
	}
}

t_env	*ft_lstfind(t_env *lst, char *name)
{

	while (lst)
	{
		if (lst->name == name)
			return (lst)
		lst = lst->next;
	}
	return (NULL);
}

void	ft_lstclear(t_env **lst)
{
	t_env	*begin;

	if (lst)
	{
		while (*lst)
		{
			begin = (*lst)->next;
			ft_lstdelone(*lst);
			*lst = begin;
		}
	}
}

void	ft_lstdelone(t_env *lst)
{
	if (lst)
		free(lst);
}
