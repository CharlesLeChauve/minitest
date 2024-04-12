/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 14:44:58 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/16 08:37:30 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*maplst;
	t_list	*act_node;
	t_list	*new_node;

	maplst = NULL;
	act_node = maplst;
	if (!lst || !f)
		return (maplst);
	while (lst)
	{
		new_node = ft_lstnew(f(lst->content));
		if (new_node == NULL)
		{
			ft_lstclear(&maplst, del);
			return (NULL);
		}
		if (maplst == NULL)
			maplst = new_node;
		else
			act_node->next = new_node;
		act_node = new_node;
		lst = lst->next;
	}
	act_node->next = NULL;
	return (maplst);
}
