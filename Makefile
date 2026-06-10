# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lucho <lucho@student.42.fr>                  +#+                      #
#                                                    +#+                       #
#    Created: 2026/01/18 11:17:27 by luimarti      #+#    #+#                  #
#    Updated: 2026/06/10 16:20:27 by dachin        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #


NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c \
	   src/signals.c \
	   src/utils.c \
	   src/cmd_execution.c \
	   src/cmd_execution2.c \
	   src/pipes.c \
	   src/pipes2.c \
	   src/builtin_env_utils.c \
	   src/builtin_cd_exit.c \
	   src/builtin_unset.c \
	   src/builtin_export.c \
	   src/builtin_export_utils.c \
	   src/builtin_echo.c \
	   src/builtin_env.c \
	   src/expander.c \
	   src/lexer.c \
	   src/parser.c \
	   src/builtin_pwd.c \
	   src/utils_lexer.c \
	   src/handlers_lexer.c \
	   src/utils_parser.c \
	   src/utils_parser_redir.c \
	   src/utils_expander.c \
	   src/utils_expander2.c \
	   src/syntax_controller.c \
	   src/core.c

OBJ_DIR = o_files
OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

LDLIBS = -lreadline

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -I/opt/homebrew/opt/readline/include
	LDLIBS += -L/opt/homebrew/opt/readline/lib
endif

all: $(NAME)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS) -L ./$(LIBFT_DIR) -lft $(LDLIBS) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -Iinclude -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) -r $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
