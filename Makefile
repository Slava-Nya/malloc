# Architecture detection
ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK := libft_malloc.so

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := src/inc
LIBFT_DIR := libft

CC := clang
CFLAGS := -Wall -Wextra -Werror -fPIC -MMD -I$(INC_DIR) -I$(LIBFT_DIR)/inc

ifeq ($(UNAME), Darwin)
    CFLAGS += -D DARWIN
    LDFLAGS := -dynamiclib -install_name @rpath/$(NAME)
else
    CFLAGS += -D LINUX
    LDFLAGS := -shared
endif

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Libft static lib
LIBFT := $(LIBFT_DIR)/libft.a

all: $(LINK)

# Link final shared library
$(NAME): $(OBJS) $(LIBFT)
	@echo "=== Linking $(NAME) ==="
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

# Create symbolic link
$(LINK): $(NAME)
	ln -sf $(NAME) $(LINK)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Build libft
$(LIBFT):
	@echo "=== calling libft make ==="
	$(MAKE) -C $(LIBFT_DIR)

# Clean rules
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(LINK)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re