NAME := test_exec

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC := clang
CFLAGS := -Wall -Wextra -Werror -I$(INC_DIR) -I../src/inc -I../libft/inc
LD_PRELOAD_LIB := ../libft_malloc.so

all: test

# Build malloc lib from parent dir before test
test: $(NAME)
	LD_LIBRARY_PATH=.. LD_PRELOAD=../libft_malloc.so ./$(NAME)

# Object file rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Link test_exec without linking malloc lib (uses LD_PRELOAD instead)
$(NAME): $(OBJS)
	@echo "Linking $(NAME)"
	$(CC) -L.. -l:libft_malloc.so $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all test clean fclean re
