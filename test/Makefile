NAME = test_malloc

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC = clang
CCFLAGS = -Wall -Wextra -Werror -I$(INC_DIR) -I../src/inc -I../libft/inc

LIBFT_MALLOC = ../libft_malloc.so
LIBFT = ../libft/libft.a

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

all:
	$(MAKE) -C ..
	$(MAKE) $(NAME)

$(NAME): $(OBJ) $(LIBFT_MALLOC) $(LIBFT)
	$(CC) $(CCFLAGS) -o $(NAME) $(OBJ) $(LIBFT_MALLOC) $(LIBFT)

run: all
ifeq ($(shell uname), Darwin)
	DYLD_LIBRARY_PATH=.. ./$(NAME)
else
	LD_PRELOAD=$(LIBFT_MALLOC) ./$(NAME)
endif

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run