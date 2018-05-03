##
## EPITECH PROJECT, 2018
## k
## File description:
## k
##

CC      = 	gcc

NAME    = 	server

SRC     =	src/main.c \
		src/check_args.c \
		src/commands.c \
		src/commands2.c \
		src/commands3.c \
		src/commands4.c \
		src/commands5.c

OBJ     = $(SRC:.c=.o)

RM      = rm -f

CFLAGS	+=	-W -Wall -Wextra -pedantic -I./include/

all:    $(NAME)

$(NAME):        $(OBJ)
	$(CC) -g $(SRC) -o $(NAME) $(CFLAGS)

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(NAME) $(OBJS)

re: fclean all

.PHONY: all clean fclean re
