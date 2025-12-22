/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_mlx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 14:07:22 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

int	setup_mlx(t_vars *vars)
{
	vars->mlx = mlx_init();
	if (!vars->mlx)
	{
		ft_putstr_fd("Erro: mlx_init falhou\n", 2);
		return (0);
	}
	init_screen_size(vars);
	vars->win = mlx_new_window(vars->mlx, vars->screen.width, 
			vars->screen.height, "FDF Quaternion - WASD/QE/Arrows");
	if (!vars->win)
	{
		ft_putstr_fd("Erro: mlx_new_window falhou\n", 2);
		mlx_destroy_display(vars->mlx);
		free(vars->mlx);
		return (0);
	}
	vars->screen.img = mlx_new_image(vars->mlx, vars->screen.width, 
			vars->screen.height);
	if (!vars->screen.img)
	{
		ft_putstr_fd("Erro: mlx_new_image falhou\n", 2);
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
