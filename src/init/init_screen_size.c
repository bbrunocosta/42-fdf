/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_screen_size.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 12:04:41 by bcosta-b         ###   ########.fr       */
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
	if (screen_w > WINDOW_WIDTH)
		screen_w = WINDOW_WIDTH;
	if (screen_h > WINDOW_HEIGHT)
		screen_h = WINDOW_HEIGHT;
	vars->screen.width = screen_w;
	vars->screen.height = screen_h;
	usable_width = vars->screen.width - (2 * MARGIN);
	usable_height = vars->screen.height - (2 * MARGIN);
	projected_width = vars->point_map.width - 1;
	projected_height = vars->point_map.height - 1;
	if (projected_width == 0)
		projected_width = 1;
	if (projected_height == 0)
		projected_height = 1;
	scale_x = usable_width / projected_width;
	scale_y = usable_height / projected_height;
	vars->screen.size = (scale_x < scale_y) ? scale_x : scale_y;
	vars->screen.offset_x = vars->screen.width / 2;
	vars->screen.offset_y = vars->screen.height / 2;
}
