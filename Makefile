CC      =   gcc

CFLAGS  =    -g -Iheaders -Wall -Wextra -Werror
READLINE_INC = -I/opt/homebrew/opt/readline/include
READLINE_LIB = -L/opt/homebrew/opt/readline/lib -lreadline

LK_FLAG =   -fsanitize=address -fsanitize=leak

NAME    =   minishell

SRCS	=	srcs/parsing.c \
			srcs/signal.c \
			srcs/tokenization.c \
			srcs/parser.c \
			srcs/utils_check.c \
			srcs/env.c \
			srcs/dirs_handler.c \
			srcs/expansing.c \
			srcs/exec.c \
			srcs/pipes.c \
			srcs/tab_utils.c \
			srcs/path_utils.c \
			srcs/redirs.c \
			srcs/file_utils.c \
			srcs/exec_actions.c \
			srcs/export.c \
			srcs/env_utils.c \
			srcs/exit.c \
			srcs/echo.c \
			srcs/new_heredoc.c \
			srcs/expand_wildcard.c \
			srcs/tab_utils2.c \
			srcs/exec_utils.c \
			srcs/type_utils.c \
			srcs/ft_sort_wordlist.c \
			srcs/match_pattern.c \
			srcs/wildcards_get_utils.c \
			srcs/parser_utils.c \
			srcs/expansing_redirs.c \
			srcs/expansing_redirs_utils.c \
			srcs/expansing_extrapolation.c \
			srcs/expansing_cmd.c \
			srcs/expansing_subtoken.c \
			srcs/tokenization_subshell.c \
			srcs/tokenization_utils.c \
			srcs/tokenization_setup.c \
			srcs/utils.c \
			srcs/export_utils.c \
			srcs/heredoc_signal.c \
			srcs/heredoc_utils.c \
			srcs/exit_utils.c \
			srcs/core_utils.c \
			srcs/clean.c


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
