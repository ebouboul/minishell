# Define variables
NAME = minishell
SRC_NAME = tokenizer.c main.c parser.c parser_space_quotes.c env.c expander.c builtins.c fill_node.c execution_v1.c free.c execution_v2.c export.c 
OBJ_NAME = $(SRC_NAME:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address -g3
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INC = $(LIBFT_DIR)/libft.h

# Define the rules
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
