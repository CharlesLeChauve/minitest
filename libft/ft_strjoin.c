/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:44:37 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/04 18:16:58 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char const *s1, char const *s2, int flag)
{
	size_t	size;
	char	*result;

	size = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)malloc ((size + 1) * sizeof (char));
	if (result == NULL)
		return (NULL);
	ft_strlcpy(result, s1, ft_strlen(s1) + 1);
	if (flag == 0 || flag == 2)
		free(s1);
	ft_strlcat(result, s2, size + 1);
	if (flag == 1 || flag == 2)
		free(s2);
	return (result);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	size;
	char	*result;

	size = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)malloc ((size + 1) * sizeof (char));
	if (result == NULL)
		return (NULL);
	ft_strlcpy(result, s1, ft_strlen(s1) + 1);
	ft_strlcat(result, s2, size + 1);
	return (result);
}
