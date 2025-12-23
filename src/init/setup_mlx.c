/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:25:46 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

static int	print_mlx_init_error(void)
{
	ft_putstr_fd("Error: mlx_init failed\n", 2);
	return (0);
}

static int	print_mlx_win_error(t_vars *vars)
{
	ft_putstr_fd("Error: mlx_new_window failed\n", 2);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	return (0);
}

static int	print_mlx_image_error(t_vars *vars)
{
	ft_putstr_fd("Error: mlx_new_image failed\n", 2);
	mlx_destroy_window(vars->mlx, vars->win);
	mlx_destroy_display(vars->mlx);
	free(vars->mlx);
	return (0);
}

int	setup_mlx(t_vars *vars)
{
	vars->mlx = mlx_init();
	if (!vars->mlx)
		return (print_mlx_init_error());
	init_screen_size(vars);
	vars->win = mlx_new_window(vars->mlx, vars->screen.width,
			vars->screen.height, "FDF Quaternion - WASD/QE/Arrows");
	if (!vars->win)
		print_mlx_win_error(vars);
	vars->screen.img = mlx_new_image(vars->mlx, vars->screen.width,
			vars->screen.height);
	if (!vars->screen.img)
		print_mlx_image_error(vars);
	vars->screen.addr = mlx_get_data_addr(vars->screen.img,
			&vars->screen.bits_per_pixel, &vars->screen.line_length,
			&vars->screen.endian);
	return (1);
}
