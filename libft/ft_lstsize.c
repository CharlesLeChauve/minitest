/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 17:26:48 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/05 18:14:19 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*actual;
	int		count;

	if (!lst)
		return (0);
	actual = lst;
	count = 1;
	while (actual->next != NULL)
	{
		actual = actual->next;
		count++;
	}
	return (count);
}
