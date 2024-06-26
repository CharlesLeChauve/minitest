/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:45:04 by tgibert           #+#    #+#             */
/*   Updated: 2024/02/27 14:51:59 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# include <stdlib.h>
# include "ft_printf.h"
# include "gnl/get_next_line.h"
//#include <string.h>
//# include <stdio.h>
//#include <limits.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_dlist
{
	void			*content;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}	t_dlist;

typedef struct s_reg
{
	void	**ptrs;
	int		size;
}			t_reg;

size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
void	ft_strcat(char *dest, const char *src);
t_list	*ft_lstnew(void *content);
t_dlist	*ft_dlstnew(void *content);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void	ft_lst_insert(t_list *target_node, t_list *new);
char	*ft_itoa(int n);
void	ft_strappend(char **dest, const char *src, size_t *len);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strrchr(const char *s, int c);
char	*ft_strnstr(const char *haystack, const char *needle, size_t n);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_free(char *s1, char *s2, int flag);
char	*ft_strtrim(char const *s, char const *set);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	**ft_split(char const *s, char c);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free_tab(char **tab);
void	ft_bzero(void *s, size_t n);
void	ft_putstr_fd(char const *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char const *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_dlstdelone(t_dlist *lst, void (*del)(void *));
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_dlstadd_front(t_dlist **lst, t_dlist *new);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_dlstadd_back(t_dlist **lst, t_dlist *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_dlstclear(t_dlist **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f) (void *));
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(char *s1, char *s2);
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
int		ft_isalpha(int c);
int		ft_isalnum(int c);
int		ft_isprint(int c);
int		ft_isascii(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_isspace(int c);
int		ft_lstsize(t_list *lst);
int		ft_sprintf(char *str, const char *input, ...);
char	*ft_strndup(const char *s, size_t n);
t_reg	*get_main_reg(void);
void	ft_free_all(void);
void	*ft_malloc(size_t size);
void	*ft_calloc(size_t count, size_t size);
void	ft_free(void *ptr);
void	*ft_realloc2(void *ptr, size_t size);
char	*ft_strncpy(char *s1, char *s2, int len);

#endif //LIBFT_H
