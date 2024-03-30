NAME	= philo
CC		= CC
CFLAGS	= -Wall -Wextra -Werror
SFLAGS	= -g3 -fsanitize=address

FILES	= srcs/main

CFILES	= $(addsuffix .c, $(FILES))
OBJS	= $(addsuffix .o, $(FILES))
HEADERS	= includes/
LIBFT_DIR	= utils

all: $(NAME)

.c.o: $(CFILES)
	$(CC) $(CFLAGS) -I $(HEADERS) -c $< -o $@

$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIBFT_DIR) -lft

clean:
	rm -rf $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf *.dSYM
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re