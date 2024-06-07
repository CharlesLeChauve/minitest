/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:52:32 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/10 12:44:55 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static size_t	getsize(char const *s, char c)
{
	size_t	size;

	size = 0;
	while (*s && *s != c && *s != '\0')
	{
		size++;
		s++;
	}
	return (size);
}

static void	*del(char **arr, int j)
{
	while (j > 0)
	{
		j--;
		free(arr[j]);
		arr[j] = NULL;
	}
	free(arr);
	return (NULL);
}

static char	**get_words(char **arr, char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	word_size;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			word_size = getsize(&s[i], c);
			arr[j] = ft_substr(&s[i], 0, word_size);
			if (!arr[j])
			{
				del(arr, j);
				return (NULL);
			}
			i += word_size;
			j++;
		}
		else
			i++;
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	arr = (char **) ft_calloc(ft_count(s, c) + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	get_words(arr, s, c);
	return (arr);
}
