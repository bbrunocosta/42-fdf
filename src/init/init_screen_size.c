/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_screen_size.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:18:20 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

void	init_screen_size(t_vars *vars)
{
	int		screen_w;
	int		screen_h;
	double	scale;

	mlx_get_screen_size(vars->mlx, &screen_w, &screen_h);
	if (screen_w > 2560)
		screen_w = 2560;
	if (screen_h > 1440)
		screen_h = 1440;
	vars->screen.width = screen_w * 0.8;
	vars->screen.height = screen_h * 0.8;
	scale = fmin(
			(vars->screen.width * 0.85)
			/ ((vars->point_map.width + vars->point_map.height) * 0.866),
			(vars->screen.height * 0.85)
			/ ((vars->point_map.width + vars->point_map.height) * 0.5)
			);
	vars->screen.size = scale;
	vars->screen.offset_x = vars->screen.width / 2;
	vars->screen.offset_y = vars->screen.height / 2.5;
}
