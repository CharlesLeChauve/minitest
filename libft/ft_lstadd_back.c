/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 09:55:47 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/11 23:56:10 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstadd_back(t_dlist **lst, t_dlist *new)
{
	t_dlist	*last_node;

	if (!*lst)
	{
		new->next = NULL;
		new->prev = NULL;
		*lst = new;
	}
	else
	{
		last_node = *lst;
		while (last_node->next)
			last_node = last_node->next;
		new->prev = last_node;
		last_node->next = new;
		new->next = NULL;
	}
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last_node;

	if (!*lst)
		*lst = new;
	else
	{
		last_node = *lst;
		while (last_node->next)
			last_node = last_node->next;
		last_node->next = new;
	}
}
