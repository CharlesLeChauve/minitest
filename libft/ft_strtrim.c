/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:44:50 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/09 13:44:20 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isinset(char c, char const *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset ++;
	}
	return (0);
}

char	*ft_strtrim(char const *s, char const *set)
{
	int		starti;
	int		endi;
	int		i;
	char	*result;

	starti = 0;
	endi = ft_strlen(s) - 1;
	i = 0;
	while (ft_isinset(s[starti], set) && starti < endi)
		starti++;
	while (ft_isinset(s[endi], set) && endi >= starti)
		endi--;
	result = (char *)malloc((endi - starti + 2) * sizeof(char));
	if (result == NULL)
		return (NULL);
	while (starti + i <= endi)
	{
		result[i] = s[starti + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
