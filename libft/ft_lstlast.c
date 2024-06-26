/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 17:33:19 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/05 18:12:52 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*last;

	last = lst;
	if (!lst)
		return (NULL);
	while (last->next != NULL)
	{
		last = last->next;
	}
	return (last);
}

t_dlist	*ft_dlstlast(t_dlist *lst)
{
	t_dlist	*last;

	last = lst;
	if (!lst)
		return (NULL);
	while (last->next != NULL)
	{
		last = last->next;
	}
	return (last);
}
