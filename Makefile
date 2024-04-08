CC		=	gcc

CFLAGS	=	-Wall -Wextra -Werror -g -lreadline

LK_FLAG	=	-fsanitize=address -fsanitize=leak

NAME	=	minishell

SRCS	=	parsing.c

BNS_SRC	=	pipex_bonus.c utils.c utils_bonus.c

OBJS	=	${SRCS:.c=.o}

BNS_OBJ	=	${BNS_SRC:.c=.o}

all		:	${NAME}

${NAME}	:	${OBJS}
			@${MAKE} -C libft
			@${CC} ${CFLAGS} ${OBJS} -o ${NAME} -L./libft/ -lft
			@echo "\033[92mProgram Compiled Successfully\033[0m"

bonus	:	${BNS_OBJ}
			@${MAKE} -C libft
			@${CC} ${CFLAGS} ${BNS_OBJ} -o ${NAME} -L./libft/ -lft
			@echo "\033[92mBonus program successfully compiled\033[0m"


sanit	:	${OBJS}
			@${MAKE} -C libft
			@${CC} ${CFLAGS} ${LK_FLAGS} ${OBJS} -o ${NAME} -L./libft/ -lft

%.o		:	%.c
			@${CC} ${CFLAGS} -c $< -o $@

clean	:
			@${MAKE} -C libft clean
			@rm -rf ${OBJS} ${BNS_OBJ}
			@echo "\033[92mCleaned objects\033[0m"

fclean	:	clean
			@${MAKE} -C libft fclean
			@rm -rf ${NAME}
			@echo "\033[92mExecutable Removed\033[0m"

re		:	fclean	all

.PHONY	:	all clean fclean re