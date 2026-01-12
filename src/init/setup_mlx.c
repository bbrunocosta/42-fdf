/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 19:39:29 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

static int	init_window(t_vars *vars)
{
	vars->win = mlx_new_window(vars->mlx, vars->screen.width,
			vars->screen.height, "42 - FDF");
	if (!vars->win)
	{
		ft_putstr_fd("Erro: mlx_new_window failed\n", 2);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
		return (0);
	}
	return (1);
}

static int	init_image(t_vars *vars)
{
	vars->screen.img = mlx_new_image(vars->mlx, vars->screen.width,
			vars->screen.height);
	if (!vars->screen.img)
	{
		ft_putstr_fd("Erro: mlx_new_image failed\n", 2);
		mlx_destroy_window(vars->mlx, vars->win);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
		return (0);
	}
	vars->screen.addr = mlx_get_data_addr(vars->screen.img,
			&vars->screen.bits_per_pixel, &vars->screen.line_length,
			&vars->screen.endian);
	return (1);
}

int	setup_mlx(t_vars *vars)
{
	vars->mlx = mlx_init();
	if (!vars->mlx)
	{
		ft_putstr_fd("Erro: mlx_init failed\n", 2);
		return (0);
	}
	init_screen_size(vars);
	if (!init_window(vars))
		return (0);
	if (!init_image(vars))
		return (0);
	return (1);
}
