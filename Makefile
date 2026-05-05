# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luimarti <luimarti@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/18 11:17:27 by luimarti          #+#    #+#              #
#    Updated: 2026/05/01 18:53:35 by luimarti         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

MANDATORY_SRC = main.c \
				echo.c \
				env.c \
				expander.c \
				lexer.c \
				parser.c \
				pwd.c \
				utils_lexer.c \
				handlers_lexer.c \
				utils_parser.c \
				utils_parser_redir.c \
				syntax_controller.c
				
OBJ_DIR = o_files
MANDATORY_OBJ = $(addprefix $(OBJ_DIR)/, $(MANDATORY_SRC:.c=.o))

RM = rm -f

all: $(NAME)

libft/libft.a:
	$(MAKE) -C libft

$(NAME): $(MANDATORY_OBJ) libft/libft.a
	$(CC) $(CFLAGS) $(MANDATORY_OBJ) -L ./libft -lft -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	@echo "🧹 Object files removed"

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C libft fclean
	@echo "🗑️  Full clean done"

re: fclean all

.PHONY: all clean fclean re