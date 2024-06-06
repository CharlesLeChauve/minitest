/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anporced <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 18:51:12 by pfilippi          #+#    #+#             */
/*   Updated: 2024/06/06 09:15:37 by anporced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

t_reg *get_main_reg(void)
{
	static t_reg reg = {NULL, 0};
	return &reg;
}

void ft_free_all(void)
{
	t_reg *reg = get_main_reg();
	int	i;

	i = -1;
	while (++i < reg->size)
		free(reg->ptrs[i]);
	free(reg->ptrs);
}

void *ft_malloc(size_t size)
{
	t_reg *reg;
	void *ptr;

	reg = get_main_reg();
	ptr = malloc(size);
	if (ptr == NULL)
		return (NULL);
	reg->ptrs = realloc(reg->ptrs, (reg->size + 1) * sizeof(void *));
	if (reg->ptrs == NULL)
	{
		free(ptr);
		return (NULL);
	}
	reg->ptrs[reg->size++] = ptr;
	return (ptr);
}

void *ft_calloc(size_t count, size_t size)
{
	void *ptr;
	size_t i;

	ptr = ft_malloc(count * size);
	i = -1;
	if (ptr == NULL)
		return (NULL);
	while (++i < count * size)
		((char *)ptr)[i] = 0;
	return (ptr);
}

void ft_free(void *ptr)
{
	t_reg *reg;
	int i;
	int j;

	i = -1;
	reg = get_main_reg();
	if (ptr == NULL)
		return ;
	while (++i < reg->size) {
		if (reg->ptrs[i] == ptr)
		{
			free(ptr);
			j = i;
			while (j < reg->size - 1)
			{
				reg->ptrs[j] = reg->ptrs[j + 1];
				j++;
			}
			reg->ptrs = realloc(reg->ptrs, (reg->size - 1) * sizeof(void *));
			reg->size--;
			return ;
		}
	}
	free(ptr);
	free(ptr);
}

void *ft_realloc2(void *ptr, size_t size)
{
	t_reg *reg;
	int		i;
	void *new_ptr;

	reg = get_main_reg();
	i = -1;
	if (ptr == NULL)
		return (ft_malloc(size));
	while (++i < reg->size)
	{
		if (reg->ptrs[i] == ptr)
		{
			new_ptr = realloc(ptr, size);
			if (new_ptr == NULL)
				return (NULL);
			reg->ptrs[i] = new_ptr;
			return (new_ptr);
		}
	}
	printf("Error: trying to realloc a non-allocated pointer\n");
	return (NULL);
}
