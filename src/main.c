/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:50:05 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "mlx.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

static void	init_screen_size(t_vars *vars)
{
	int		screen_w;
	int		screen_h;
	double	usable_width;
	double	usable_height;
	double	projected_width;
	double	projected_height;
	double	scale_x;
	double	scale_y;

	mlx_get_screen_size(vars->mlx, &screen_w, &screen_h);
	if (screen_w > 2560)
		screen_w = 2560;
	if (screen_h > 1440)
		screen_h = 1440;
	vars->screen.width = screen_w * 0.8;
	vars->screen.height = screen_h * 0.8;
	usable_width = vars->screen.width * 0.85;
	usable_height = vars->screen.height * 0.85;
	projected_width = vars->point_map.width * 0.866 + vars->point_map.height * 0.866;
	projected_height = vars->point_map.width * 0.5 + vars->point_map.height * 0.5;
	scale_x = usable_width / projected_width;
	scale_y = usable_height / projected_height;
	vars->screen.size = (scale_x < scale_y) ? scale_x : scale_y;
	vars->screen.offset_x = vars->screen.width / 2;
	vars->screen.offset_y = vars->screen.height / 2.5;
}

static void	init_vars(t_vars *vars)
{
	vars->quat = quat_identity();
	vars->rotation.x = 0;
	vars->rotation.y = 0;
	vars->rotation.z = 0;
	vars->translation.x = 0;
	vars->translation.y = 0;
	vars->zoom = 1.0;
	vars->needs_render = 1;
	memset(&vars->keys, 0, sizeof(t_keys));
}

static int	setup_mlx(t_vars *vars)
{
	vars->mlx = mlx_init();
	if (!vars->mlx)
	{
		printf("Erro: mlx_init falhou\n");
		return (0);
	}
	init_screen_size(vars);
	vars->win = mlx_new_window(vars->mlx, vars->screen.width, 
			vars->screen.height, "FDF Quaternion - WASD/QE/Arrows");
	if (!vars->win)
	{
		printf("Erro: mlx_new_window falhou\n");
		return (0);
	}
	vars->screen.img = mlx_new_image(vars->mlx, vars->screen.width, 
			vars->screen.height);
	vars->screen.addr = mlx_get_data_addr(vars->screen.img, 
			&vars->screen.bits_per_pixel, &vars->screen.line_length, 
			&vars->screen.endian);
	return (1);
}

static int	parse_map(t_vars *vars, char *filename)
{
	int		fd;
	char	*content;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (0);
	}
	content = read_file_to_string(fd);
	if (!content)
	{
		perror("read_file_to_string");
		close(fd);
		return (0);
	}
	if (!try_parse(vars, content))
	{
		printf("Erro: falha ao analisar o mapa\n");
		free(content);
		close(fd);
		return (0);
	}
	free(content);
	close(fd);
	return (1);
}

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc < 2)
	{
		fprintf(stderr, "usage: %s <file.fdf>\n", argv[0]);
		return (1);
	}
	memset(&vars, 0, sizeof(t_vars));
	if (!parse_map(&vars, argv[1]))
		return (1);
	if (!setup_mlx(&vars))
		return (1);
	init_vars(&vars);
	mlx_hook(vars.win, 17, 0, close_window, &vars);
	mlx_hook(vars.win, 2, 1L << 0, handle_keypress, &vars);
	mlx_hook(vars.win, 3, 1L << 1, handle_keyrelease, &vars);
	mlx_loop_hook(vars.mlx, update_and_render, &vars);
	render(&vars);
	mlx_loop(vars.mlx);
	return (0);
}
