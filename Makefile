# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brunocosta                                 +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/12                           #+#    #+#              #
#    Updated: 2025/11/12                           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nome do executável
NAME = fdf

# Compilador e flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I. -I./minilibx-linux

# Diretórios
MLX_DIR = ./minilibx-linux
SRC_DIR = .
OBJ_DIR = obj

# Arquivos fonte
SRCS = main.c

# Arquivos objeto
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Biblioteca MiniLibX
MLX = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# Cores para output
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

# Regra principal
all: $(MLX) $(NAME)

# Compilar o executável
$(NAME): $(OBJS) $(MLX)
	@echo "$(GREEN)Compilando $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compilado com sucesso!$(RESET)"

# Compilar objetos
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(GREEN)Compilando $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilar MiniLibX
$(MLX):
	@echo "$(GREEN)Compilando MiniLibX...$(RESET)"
	@cd $(MLX_DIR) && ./configure > /dev/null 2>&1 || true
	@make -C $(MLX_DIR) -f Makefile.gen > /dev/null 2>&1 || make -C $(MLX_DIR) all > /dev/null 2>&1
	@echo "$(GREEN)✓ MiniLibX compilada!$(RESET)"

# Limpar objetos
clean:
	@echo "$(RED)Removendo objetos...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make -C $(MLX_DIR) clean > /dev/null 2>&1
	@echo "$(RED)✓ Objetos removidos!$(RESET)"

# Limpar tudo
fclean: clean
	@echo "$(RED)Removendo $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED)✓ $(NAME) removido!$(RESET)"

# Recompilar tudo
re: fclean all

# Executar com mapa de teste
test: all
	@./$(NAME) test_maps/42.fdf

# Verificar vazamentos de memória
valgrind: all
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) test_maps/42.fdf

# Phony targets
.PHONY: all clean fclean re test valgrind
