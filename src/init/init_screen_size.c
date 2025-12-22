/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_screen_size.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 12:57:49 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

void	init_screen_size(t_vars *vars)
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
