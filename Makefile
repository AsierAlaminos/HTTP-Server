NAME = server
CCFLAGS = -Wall -Wextra -Werror
FSANITIZE = -fsanitize=address

FILES = src/main.c

OBJS = $(FILES:.c=.o)

all: $(NAME)

lib: $(LIBFT)

$(NAME): $(OBJS)
	cc $(CCFLAGS) $(FILES) -o $(NAME)

.o: .c
	cc $(CCFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re lib
