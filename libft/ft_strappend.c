#include "libft.h"

void	ft_strappend(char **dest, const char *src, size_t *len)
{
	size_t	src_len;
	char	*new_str;

	if (!src)
		return ;
	src_len = ft_strlen(src);
	if (*dest == NULL)
	{
		*dest = (char *)malloc(src_len + 1);
		if (!*dest)
		{
			perror("Allocation Failed");
			exit(EXIT_FAILURE);
		}
		ft_strcpy(*dest, src);
		*len = src_len;
	}
	else
	{
		new_str = (char *)malloc(*len + src_len + 1);
		if (!new_str)
		{
			perror("Allocation Failed");
			exit(EXIT_FAILURE);
		}
		ft_strcpy(new_str, *dest);
		ft_strcat(new_str, src);
		free(*dest);
		*dest = new_str;
		*len += src_len;
	}
}