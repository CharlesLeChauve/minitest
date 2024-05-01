/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 10:10:04 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/06 11:29:10 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*act_node;
	t_list	*next;

	act_node = *lst;
	while (act_node)
	{
		next = act_node->next;
		del(act_node->content);
		free(act_node);
		act_node = next;
	}
	*lst = NULL;
}

void	ft_dlstclear(t_dlist **lst, void (*del)(void *))
{
	ft_lstclear((t_list **)lst, del);
}
