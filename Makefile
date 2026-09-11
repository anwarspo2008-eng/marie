# --- Compiler & Flags ---
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -Iincludes
RM      := rm -rf

# --- Program Name ---
NAME    := marie

# --- Directories ---
SRC_DIR := src
OBJ_DIR := obj

# --- Source & Object Files ---
# Automatically find all .c files in the src/ directory
SRCS    := $(wildcard $(SRC_DIR)/*.c)
# Convert src/file.c -> obj/file.o
OBJS    := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# --- Primary Rule ---
all: $(NAME)

# --- Link Object Files into Final Executable ---
$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Build successful! Run with ./\033[1;32m$(NAME)\033[0m"

# --- Compile .c Files into .o Object Files inside obj/ ---
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# --- Create obj/ Directory if it doesn't exist ---
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# --- Clean Up Built Files ---
clean:
	@echo "Cleaning object files..."
	$(RM) $(OBJ_DIR)

fclean: clean
	@echo "Cleaning executable..."
	$(RM) $(NAME)

re: fclean all

# --- Declare Targets that are not Files ---
.PHONY: all clean fclean re