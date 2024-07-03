/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_wordlist.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 10:49:55 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/03 17:10:18 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcasecmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && ft_tolower(*s1) == ft_tolower(*s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2));
}

void	ft_lstadd_sorted(t_list **head, t_list *new_node, \
	int (*cmp)(const char*, const char*))
{
	t_list	*current;

	if (*head == NULL || \
		cmp((char *)(*head)->content, (char *)new_node->content) > 0)
	{
		new_node->next = *head;
		*head = new_node;
	}
	else
	{
		current = *head;
		while (current->next != NULL && \
			cmp((char *)current->next->content, \
			(char *)new_node->content) <= 0)
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

void	ft_sort_wordlist(t_list **head)
{
	t_list	*sorted;
	t_list	*current;
	t_list	*next_node;

	sorted = NULL;
	current = *head;
	while (current != NULL)
	{
		next_node = current->next;
		ft_lstadd_sorted(&sorted, current, ft_strcasecmp);
		current = next_node;
	}
	*head = sorted;
}
