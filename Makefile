.SILENT:

NAME = pipex
CC = cc
CFLAGS = -Werror -Wextra -Wall

LIBFT = libft/
INC = includes/pipex.h
SRCS = srcs/main.c srcs/utils.c
OBJS = $(SRCS:.c=.o)

DEFAULT = \033[0m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m


all : $(NAME) Makefile

$(NAME) : $(LIBFT) $(SRCS) $(OBJS) $(LIBFT) Makefile
	make --no-print-directory -C $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT)libft.a -o $(NAME)

clean : 
	make fclean --no-print-directory -C $(LIBFT)
	rm -rf $(OBJS)

fclean : clean
	rm $(NAME)

re : fclean all

.PHONY: all clean fclean re
