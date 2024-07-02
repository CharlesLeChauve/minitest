/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 17:15:22 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/05 18:13:44 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstadd_front(t_dlist **lst, t_dlist *new)
{
	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		new->prev = NULL;
	}
	else
	{
		new->prev = NULL;
		(*lst)->prev = new;
		new->next = *lst;
		*lst = new;
	}
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new)
		return ;
	if (!*lst)
	{
		new->next = NULL;
		*lst = new;
	}
	else
	{
		new->next = *lst;
		*lst = new;
	}
}
