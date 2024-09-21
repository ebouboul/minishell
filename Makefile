
###################
NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -fsanitize=address -g3
LDFLAGS = -I/usr/local/opt/readline/include
RDFLAG = -L/usr/local/opt/readline/lib -lreadline
RM = rm -rf
LIBFT = ./libft/libft.a
###################
OBGDIR = obj
###################
# Your SRC files
SRC = main.c \
	execution/execute_heredocP1.c execution/execute_heredocP2.c execution/execution_v1.c \
	execution/execution_v2.c execution/execute_pipe_and_cmds.c execution/execute_redirectionsP1.c \
	execution/execute_redirectionsP2.c execution/execution_v1_1.c execution/exect.c execution/execution_v1_2.c \
	bultins/echo.c bultins/cd.c bultins/export.c bultins/unset.c bultins/ft_env.c \
	bultins/ft_exit.c bultins/builtins.c bultins/export2.c \
	pars/checking3.c  pars/env.c pars/fill_node.c pars/wildcards1.c pars/parser.c pars/parser_space_quotes.c \
	pars/tokenizer.c pars/Wildcards.c pars/quotes.c pars/checking.c pars/checking2.c \
	pars/tokenizer2.c expander/expander1.c expander/expander2.c expander/expander3.c \
	expander/expander4.c expander/expander5.c expander/expander6.c expander/expander7.c \
	utils/utils.c utils/utils1.c utils/utils2.c utils/signals.c utils/free.c utils/free1.c
####################

# Object files
OBJ = $(addprefix $(OBGDIR)/, $(notdir $(SRC:.c=.o)))

# Object directory creation and compilation rules
$(OBGDIR)/%.o:%.c
	@mkdir -p $(OBGDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBGDIR)/%.o:execution/%.c
	@mkdir -p $(OBGDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBGDIR)/%.o:bultins/%.c
	@mkdir -p $(OBGDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBGDIR)/%.o:pars/%.c
	@mkdir -p $(OBGDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBGDIR)/%.o:expander/%.c
	@mkdir -p $(OBGDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBGDIR)/%.o:utils/%.c
	@mkdir -p $(OBGDIR)
	$(CC) $(CFLAGS) -c $< -o $@

####################
all: $(NAME)

$(LIBFT):
	$(MAKE) --no-print-directory -C ./libft

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJ) $(RDFLAG) $(LIBFT) -o $(NAME)
	@echo "============================="
	@echo "      Minishell Created      "
	@echo "============================="

clean:
	@$(RM)  -rf $(OBGDIR)
	@$(MAKE) --no-print-directory -C ./libft clean
	@echo "============================="
	@echo "    Object files removed.    "
	@echo "============================="

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) --no-print-directory -C ./libft fclean
	@echo "============================="
	@echo "      Minishell removed      "
	@echo "============================="

re: fclean all

.SECONDARY: $(OBJ)
.SILENT:
