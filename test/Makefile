NAME := test_exec
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

CC := clang
CFLAGS := -Wall -Wextra -Werror -I$(INC_DIR) -I../src/inc -I../libft/inc
LDFLAGS := -L.. -lft_malloc

UNAME := $(shell uname)

test: build_malloc $(NAME)
ifeq ($(UNAME), Darwin)
	@echo "Running with DYLD_INSERT_LIBRARIES..."
	DYLD_FORCE_FLAT_NAMESPACE=1 DYLD_INSERT_LIBRARIES=../libft_malloc.so ./$(NAME)
else
	@echo "Running with LD_PRELOAD..."
	LD_PRELOAD=../libft_malloc.so ./$(NAME)
endif

build_malloc:
	@echo "=== Building malloc lib ==="
	@$(MAKE) -C ..

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo "Linking test_exec"
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean test

-include $(DEPS)

.PHONY: test build_malloc clean fclean re