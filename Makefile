# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/24 15:11:31 by hhadhadi          #+#    #+#              #
#    Updated: 2024/06/06 13:11:19 by hhadhadi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CFILES = main.c ./lexer/lexer.c ./lexer/lexer_utils.c ./lexer/lexer_utils2.c \
		./syntax/syntax.c ./syntax/syntax_utils.c \
		./parsing/parse.c

OBJ = $(CFILES:.c=.o)

CC = cc

CFLAGS = -Werror -Wall -Wextra -fsanitize=address -g

INLUCDE = ./include/minishell.h

GREEN = \033[0;32m
YELLOW =\033[0;33m
RED = \033[0;31m
GRAY = \033[33;2;37m
RESET = \033[0m

%.o: %.c $(INCLUDE)
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(GREEN)    - Building $(NAME)$(RESET)"
	@make -C libft
	@$(CC) $(CFLAGS) $(OBJ) ./libft/libft.a -lreadline -lhistory -o $(NAME)
	@echo "$(GREEN)    - Executable ready.$(RESET)"

clean:
	@make clean -C libft
	@rm -rf $(OBJ)
	@echo "$(GRAY)	- Removing object files ... $(RESET)"
	@echo "$(RED)    - Object files removed.$(RESET)"

fclean: clean
	@make fclean -C libft
	@rm -rf $(NAME)
	@echo "$(GRAY)	- Removing $(NAME)... $(RESET)"
	@echo "$(RED)    - Executable removed.$(RESET)"

re: fclean all
	@echo "$(YELLOW)Rebuilding Finished$(RESET)"

# UTILS		= $(addprefix utils/, ft_atoi ft_itoa ft_split ft_calloc ft_realloc get_line string string_utils)
# LEXER		= $(addprefix lexer/, list lexer lexer_utils)
# PARSER		= $(addprefix parser/, parser ast redir_utils parse_core parse_utils)
# ENV			= $(addprefix env/, env_list env_utils env_search)
# BUILINS		= $(addprefix builtins/, $(ENV) ft_export ft_unset ft_pwd ft_echo echo_utils ft_cd ft_exit ft_env export_utils_0 export_utils_1)
# EXEC		= $(addprefix exec/, execute redir pipe find execute_cmd)
# FILES		= $(addprefix srcs/, minishell main_utils signals error clean_up syntax $(UTILS) $(LEXER) $(PARSER) $(BUILINS) $(EXEC))

# SRC		= $(FILES:=.c)
# OBJ		= $(FILES:=.o)
# HEADER		= $(addprefix includes/, minishell.h env.h)
