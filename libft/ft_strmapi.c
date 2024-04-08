/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:44:41 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/04 20:07:44 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	size;
	size_t	i;
	char	*result;

	size = ft_strlen(s);
	result = (char *) malloc((size + 1) * sizeof (char));
	if (!result)
		return (NULL);
	i = 0;
	while (s[i])
	{
		result[i] = f((unsigned int)i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
