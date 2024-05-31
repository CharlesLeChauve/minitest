CC      =   gcc

CFLAGS  =    -g -Iheaders/
READLINE_INC = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib -lreadline

LK_FLAG =   -fsanitize=address -fsanitize=leak

NAME    =   minishell

SRCS    =   parsing.c token.c signal.c tokenization.c parser.c utils_check.c env.c dirs_handler.c expansing.c exec.c\
			GPT_works_for_me.c pipes.c

BNS_SRC =   pipex_bonus.c utils.c utils_bonus.c

OBJS    =   ${SRCS:.c=.o}

BNS_OBJ =   ${BNS_SRC:.c=.o}

all     :   ${NAME}

${NAME} :   ${OBJS}
			@${MAKE} -C libft
			@${CC} ${CFLAGS} ${OBJS} -o ${NAME} ${READLINE_INC} ${READLINE_LIB} -L./libft/ -lft
			@echo "\033[92mProgram Compiled Successfully\033[0m"

bonus   :   ${BNS_OBJ}
			@${MAKE} -C libft
			@${CC} ${CFLAGS} ${BNS_OBJ} -o ${NAME} ${READLINE_INC} ${READLINE_LIB} -L./libft/ -lft
			@echo "\033[92mBonus program successfully compiled\033[0m"

sanit   :   ${OBJS}
			@${MAKE} -C libft
			# Correction de LK_FLAG en LK_FLAGS et ajout des flags pour Readline
			@${CC} ${CFLAGS} ${LK_FLAG} ${OBJS} -o ${NAME} ${READLINE_INC} ${READLINE_LIB} -L./libft/ -lft

%.o     :   %.c
			@${CC} ${CFLAGS} ${READLINE_INC} -c $< -o $@

clean   :
			@${MAKE} -C libft clean
			@rm -rf ${OBJS} ${BNS_OBJ}
			@echo "\033[92mCleaned objects\033[0m"

fclean  :   clean
			@${MAKE} -C libft fclean
			@rm -rf ${NAME}
			@echo "\033[92mExecutable Removed\033[0m"

re      :   fclean all

.PHONY  :   all clean fclean re bonus sanit
