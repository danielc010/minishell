# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: dachin <dachin@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2026/03/18 08:09:02 by dachin        #+#    #+#                  #
#    Updated: 2026/06/03 15:36:14 by dachin        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# List of source files
SRCS = src/main.c src/signals.c src/cmd_execution.c src/pipes.c \
		src/builtin.c src/builtin2.c src/cmd_execution2.c src/pipes2.c \
		src/builtin3.c src/utils_executor.c \
		src/core.c src/lexer.c src/handlers_lexer.c src/utils_lexer.c \
		src/parser.c src/utils_parser.c src/utils_parser_redir.c \
		src/expander.c src/utils_expander.c src/syntax_controller.c \
		src/echo.c src/pwd.c src/env.c

# Creates object files by replacing .c with .o. $ references variables or automatic variables
OBJS = $(SRCS:.c=.o)

# Compiler and compiler flags.
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Library paths
LDLIBS = -lreadline
# Subproject directories
LIBFT_DIR = libft

# Subproject libraries
LIBFT_LIB = $(LIBFT_DIR)/libft.a

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	CFLAGS += -I/opt/homebrew/opt/readline/include
	LDLIBS += -L/opt/homebrew/opt/readline/lib
endif

# Main executable name
NAME = minishell

# Default target. Always put this in first place
all: $(NAME)

# Build main executable
$(NAME): $(LIBFT_LIB) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_LIB) $(LDLIBS) -o $(NAME)

# Build subproject libraries
$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

# Pattern rule. Compiles any .c file into .o using specified compiler and flags
# -I. lets root-level .c files find minishell.h
# -Iinclude lets src/ files find include/minishell.h
%.o: %.c $(LIBFT_LIB)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -Iinclude -I. -c $< -o $@

# $(RM) is predefined in make and equals rm -f. -f force flag avoids errors if files do not exist
clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

# Removes all object files and the library file.
fclean:	clean
	$(RM) $(NAME)
	$(RM) tests/test_runner
	$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuilds everything by clean and all
re:	fclean all

# Declares these targets as phony (not actual files), so Make always runs them when requested.
.PHONY: all clean fclean re test sanitize

# ===== TEST SECTION =====
# Run bash test suite
test: $(NAME)
	@echo "Running test suite..."
	@cd tests && ./test.sh

# fsanitize for macOS
sanitize: CFLAGS += -fsanitize=address -g
sanitize: re
