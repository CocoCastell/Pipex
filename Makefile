.SILENT:

# Variables

NAME = pipex
CC = cc
CFLAGS = -Werror -Wextra -Wall

LIBFT = libft/
INC = includes/pipex.h
SRCS = srcs/main.c srcs/utils.c srcs/error_manager.c
OBJS = $(SRCS:.c=.o)

BONUS_SRCS = bonus/main_bonus.c bonus/utils_bonus.c bonus/error_manager_bonus.c
BONUS_OBJS = $(BONUS_SRCS:bonus.c=bonus.o)

# Colors

DEFAULT = \033[0m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

# Rules

all : $(NAME) Makefile

$(NAME) : $(LIBFT) $(SRCS) $(OBJS) Makefile
	make --no-print-directory -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT)libft.a -o $(NAME)

bonus : $(LIBFT) $(BONUS_SRCS) $(BONUS_OBJS) Makefile
	make --no-print-directory -C $(LIBFT)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT)libft.a -o $(NAME)_bonus

clean : 
	make fclean --no-print-directory -C $(LIBFT)
	rm -rf $(OBJS) $(BONUS_OBJS)

fclean : clean
	rm $(NAME) $(NAME)_bonus

re : fclean all

.PHONY: all clean fclean re
