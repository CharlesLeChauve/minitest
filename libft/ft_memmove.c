/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:44:16 by tgibert           #+#    #+#             */
/*   Updated: 2023/10/09 11:23:26 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest_ptr;
	unsigned char	*src_ptr;

	dest_ptr = (unsigned char *) dest;
	src_ptr = (unsigned char *) src;
	if (!dest_ptr && !src_ptr)
		return (NULL);
	if (src_ptr < dest_ptr && src_ptr + n > dest_ptr)
	{
		i = n;
		while (i--)
			dest_ptr[i] = src_ptr[i];
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}
/*int main() {
    char buffer[100] = "Hello, World!";

    // Cas de chevauchement où la source est avant la destination
    char *src1 = buffer;
    char *dest1 = buffer + 7;  // Destination chevauchant source

    ft_memmove(dest1, src1, strlen(src1) + 1);

    printf("Après chevauchement src1 -> dest1 : %s\n", dest1);

	char buffer2[100] = "Hello, World!";

    // Cas de chevauchement où la destination est avant la source
    char *src2 = buffer2 + 3;  // Source chevauchant destination
    char *dest2 = buffer2;

	printf("%s\n", src2);

    ft_memmove(dest2, src2, strlen(src2) + 1);

    printf("Après chevauchement src2 -> dest2 : %s\n", dest2);

    return 0;
}*/