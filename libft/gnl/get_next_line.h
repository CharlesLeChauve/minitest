/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 09:50:14 by tgibert           #+#    #+#             */
/*   Updated: 2024/02/27 14:46:16 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif //BUFFER_SIZE
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

unsigned long	ft_strlen(const char *s);
char			*ft_strdup(const char *s);
char			*ft_substr(char const *s,
					unsigned int start, unsigned long len);
char			*ft_strnew(unsigned long len);
char			*ft_gnljoin(char *stash, char *buffer);
char			*get_next_line(int fd);

#endif //GET_NEXT_LINE_H