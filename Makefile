# Variables
NAME = minishell
LIBFT = libft/libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
SRCS = minishell.c
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft
INCLUDES = -I includes -I $(LIBFT_DIR)/includes

# Rules
all: $(LIBFT) $(NAME)

$(LIBFT):
	@echo "$(GREEN)Compiling libft...$(NC)"
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)$(NAME) is ready!$(NC)"

%.o: %.c
	@echo "$(GREEN)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(NC)"
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)Cleaning all files...$(NC)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
