CC	=	gcc

CFLAGS	=	-Wall -Wextra -Werror 

NAME	=	libft.a

SRCS	=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c\
			ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_tolower.c ft_toupper.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c\
			ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c\
			ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c  prints.c numbers.c ft_printf.c ft_sprintf.c\
			ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c\
			$(wildcard gnl/*.c)


OBJECTS		=	$(SRCS:.c=.o)

BONUSOBJ	=	$(BONUSSRCS:.c=.o)

all	: 	$(NAME)

$(NAME)	: 	$(OBJECTS)
			@ar rc $(NAME) $(OBJECTS)
			@echo "\033[92mLibft Compiled\033[0m"

%.o		:	%.c
			@${CC} ${CFLAGS} -c $< -o $@

clean	:	
			@rm -f $(OBJECTS) $(BONUSOBJ)

fclean	:	clean
			@rm -f $(NAME)
			@echo "\033[92mLibft.a Removed\033[0m"

re	:	fclean all

.PHONY	:	clean fclean all re bonus