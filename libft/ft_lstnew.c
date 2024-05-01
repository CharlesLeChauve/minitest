/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:42:37 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/05 18:05:47 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*newlst;

	newlst = (t_list *) malloc(sizeof(t_list));
	if (!newlst)
		return (NULL);
	newlst->content = content;
	newlst->next = NULL;
	return (newlst);
}

t_dlist	*ft_dlstnew(void *content)
{
	t_dlist	*newlst;

	newlst = (t_dlist *) malloc(sizeof(t_dlist));
	if (!newlst)
		return (NULL);
	newlst->content = content;
	newlst->prev = NULL;
	newlst->next = NULL;
	return (newlst);
}
