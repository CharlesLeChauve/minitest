/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstinsert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:11:58 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 10:22:35 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstinsert_after(t_list **target, t_list *node)
{
	if (!target || !node)
		return ;
	if (*target == NULL)
	{
		*target = node;
		return ;
	}
	node->next = (*target)->next;
	(*target)->next = node;
}

void	ft_lstinsert_before(t_list **target, t_list *node)
{
	t_list	*tmp;

	if (!target || !node)
		return ;
	if (*target == NULL)
	{
		*target = node;
		return ;
	}
	tmp = (t_list *)malloc(sizeof(t_list));
	if (!tmp)
		return ;
	tmp->content = (*target)->content;
	tmp->next = (*target)->next;
	(*target)->content = ft_strdup(node->content);
	(*target)->next = tmp;
}

void	ft_lstinsert_lst_replace(t_list **target, t_list *lst)
{
	t_list	*last_node;
	t_list	*temp;

	if (!target || !lst)
		return ;
	last_node = ft_lstlast(lst);
	if (*target == NULL)
		*target = lst;
	else
	{
		temp = (*target)->next;
		free((*target)->content);
		(*target)->content = lst->content;
		(*target)->next = lst->next;
		last_node->next = temp;
		free(lst);
	}
}
