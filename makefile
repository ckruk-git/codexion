CC = gcc 
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = codexion.c \
		assigner.c \
		life_cycle.c \
		utils.c \
		utils2.c \
		states.c \
		time.c \
		heap.c \
		heap2.c

OBJ = $(SRC:.c=.o)
NAME = codexion

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		/bin/rm -f $(OBJ)

fclean: clean
		/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re