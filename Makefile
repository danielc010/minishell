# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lucho <lucho@student.42.fr>                  +#+                      #
#                                                    +#+                       #
#    Created: 2026/01/18 11:17:27 by luimarti      #+#    #+#                  #
#    Updated: 2026/06/11 15:46:23 by dachin        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #


NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = src/main.c \
	   src/utils.c \
	   src/builtins/builtin_cd_exit.c \
	   src/builtins/builtin_echo.c \
	   src/builtins/builtin_env.c \
	   src/builtins/builtin_env_utils.c \
	   src/builtins/builtin_export.c \
	   src/builtins/builtin_export_utils.c \
	   src/builtins/builtin_pwd.c \
	   src/builtins/builtin_unset.c \
	   src/execution/cmd_execution.c \
	   src/execution/cmd_execution2.c \
	   src/execution/heredoc.c \
	   src/execution/pipes.c \
	   src/execution/pipes2.c \
	   src/execution/pipes3.c \
	   src/execution/signals.c \
	   src/parse/core.c \
	   src/parse/expander.c \
	   src/parse/handlers_lexer.c \
	   src/parse/lexer.c \
	   src/parse/parser.c \
	   src/parse/syntax_controller.c \
	   src/parse/utils_expander.c \
	   src/parse/utils_expander2.c \
	   src/parse/utils_lexer.c \
	   src/parse/utils_parser.c \
	   src/parse/utils_parser_redir.c

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

$(OBJ_DIR)/%.o: src/builtins/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -Iinclude -c $< -o $@

$(OBJ_DIR)/%.o: src/execution/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -Iinclude -c $< -o $@

$(OBJ_DIR)/%.o: src/parse/%.c | $(OBJ_DIR)
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
