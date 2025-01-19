NAME = pipex
CFLAGS = -Werror -Wextra -Wall

INC = includes/pipex.h
SRCS = $(wildcard srcs/*.c)

all : $(NAME)

$(NAME) :

clean :

fclean :

re :

.PHONY: all clean fclean re
