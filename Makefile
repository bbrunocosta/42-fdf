# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/12 00:00:00 by                   #+#    #+#              #
#    Updated: 2026/01/11 22:49:53 by bcosta-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fdf
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I./src -I./minilibx-linux
MLX_DIR = ./minilibx-linux
SRC_DIR = src
OBJ_DIR = obj

SRCS = src/main.c \
       src/draw_line_optimized.c \
       src/free_points.c \
       src/parser/count_tokens.c \
       src/parser/read_file_to_string.c \
       src/parser/try_parse.c \
       src/parser/is_valid_number.c \
       src/parser/parse_map.c \
       src/quaternions/quat_conjugate.c \
       src/quaternions/quat_from_axis_angle.c \
       src/quaternions/quat_identity.c \
       src/quaternions/quat_multiply.c \
       src/quaternions/quat_normalize.c \
       src/quaternions/quat_rotate_point.c \
       src/rendering/put_pixel.c \
       src/rendering/render.c \
       src/rendering/is_visible.c \
       src/projection/project_iso.c \
       src/events/handle_keypress.c \
       src/events/handle_keyrelease.c \
       src/events/close_window.c \
       src/events/update_and_render.c \
       src/events/update_rotation.c \
       src/events/update_rotation_x.c \
       src/events/update_rotation_y.c \
       src/events/update_rotation_z.c \
       src/events/update_translation.c \
       src/events/update_zoom.c \
       src/init/init_screen_size.c \
       src/init/init_vars.c \
       src/init/setup_mlx.c \
       src/utils/ft_putstr_fd.c \
       src/utils/ft_memset.c \
       src/utils/ft_atoi.c \
       src/utils/ft_strchr.c \
       src/utils/ft_strdup.c \
       src/utils/ft_atoi_base.c \
       src/utils/ft_skip_spaces.c \
       src/utils/ft_split.c \
       src/utils/ft_strjoin.c

OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)
MLX = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(NAME): $(MLX) $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MLX):
	@if [ -f $(MLX_DIR)/Makefile ] || [ -f $(MLX_DIR)/Makefile.gen ]; then \
		echo "$(GREEN)Compiling MiniLibX...$(RESET)"; \
		cd $(MLX_DIR) && ./configure > /dev/null 2>&1 || true; \
		make -C $(MLX_DIR) -f Makefile.gen > /dev/null 2>&1 || make -C $(MLX_DIR) all > /dev/null 2>&1; \
		echo "$(GREEN)✓ MiniLibX compiled!$(RESET)"; \
	else \
		echo "$(RED)Warning: MiniLibX not found in $(MLX_DIR)$(RESET)"; \
	fi

clean:
	@echo "$(RED)Removing objects...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make -C $(MLX_DIR) clean > /dev/null 2>&1 || true
	@echo "$(RED)✓ Objects removed!$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(RED)✓ $(NAME) removed!$(RESET)"

re: fclean all

test: all
	@./$(NAME) test_maps/42.fdf

valgrind: all
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) test_maps/42.fdf

.PHONY: all clean fclean re test valgrind
