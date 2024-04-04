NAME	=	philo
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror
SFLAGS	=	-g3 -fsanitize=address

FILES	=	srcs/main \
			srcs/utils \
			srcs/actions \
			srcs/handlers \
			srcs/initialisers \
			srcs/update_values \
			srcs/dinner_utils \
			srcs/dinner

CFILES	= $(addsuffix .c, $(FILES))
OBJS	= $(addsuffix .o, $(FILES))
HEADERS	= includes/

all: $(NAME)

.c.o: $(CFILES)
	$(CC) $(CFLAGS) -I $(HEADERS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ 

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf *.dSYM
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re