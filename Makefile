NAME = minishell
SRC_NAME = main.c  free.c  signals.c utils.c utils1.c ./execution/execute_heredocP1.c ./execution/execute_heredocP2.c ./execution/execution_v1.c ./execution/execution_v2.c execution/execute_pipe_and_cmds.c execution/execute_redirectionsP1.c \
./execution/execute_redirectionsP2.c ./execution/execution_v1_1.c ./execution/execution_v1_2.c \
./bultins/echo.c ./bultins/cd.c ./bultins/export.c ./bultins/unset.c ./bultins/ft_env.c ./bultins/ft_exit.c ./bultins/builtins.c ./bultins/export2.c \
./pars/env.c ./pars/expander.c ./pars/fill_node.c  ./pars/parser.c ./pars/parser_space_quotes.c ./pars/tokenizer.c ./pars/Wildcards.c ./pars/quotes.c  ./pars/checking.c ./pars/checking2.c ./pars/tokenizer2.c
OBJ_NAME = $(SRC_NAME:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address -g3
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INC = $(LIBFT_DIR)/libft.h
EXEC_DIR = execution

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ_NAME) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ_NAME) $(LIBFT) -lreadline -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ_NAME)
	@$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
