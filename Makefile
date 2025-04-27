ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = clang
CCFLAGS = -Wall -Wextra -Werror -fPIC -Ilibft/inc -Isrc/inc

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CCFLAGS += -D DARWIN
	LDFLAGS = -dynamiclib
else
	CCFLAGS += -D LINUX
	LDFLAGS = -shared
endif

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

all: $(LIBFT) $(NAME) $(LINK)

$(LIBFT):
	@echo "=== calling libft make ==="
	@$(MAKE) -C $(LIBFT_DIR) --debug=b

$(NAME): $(OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LINK): $(NAME)
	ln -sf $(NAME) $(LINK)

clean:
	rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(LINK)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re