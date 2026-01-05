/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_screen_size.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 18:20:02 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

static double	calculate_max_radius(t_vars *vars)
{
	double		center_x;
	double		center_y;
	double		center_z;
	double		max_radius;
	double		dist;
	double		dx;
	double		dy;
	double		dz;
	unsigned int	y;
	unsigned int	x;

	center_x = (vars->point_map.width - 1) / 2.0;
	center_y = (vars->point_map.height - 1) / 2.0;
	center_z = 0;
	max_radius = 0;
	y = 0;
	while (y < vars->point_map.height)
	{
		x = 0;
		while (x < vars->point_map.width)
		{
			dx = x - center_x;
			dy = y - center_y;
			dz = vars->point_map.points[y][x].z - center_z;
			dist = sqrt(dx * dx + dy * dy + dz * dz);
			if (dist > max_radius)
				max_radius = dist;
			x++;
		}
		y++;
	}
	return (max_radius);
}

void	init_screen_size(t_vars *vars)
{
	int		screen_w;
	int		screen_h;
	double	usable_width;
	double	usable_height;
	double	max_radius;
	double	usable_size;

	mlx_get_screen_size(vars->mlx, &screen_w, &screen_h);
	if (screen_w > WINDOW_WIDTH)
		screen_w = WINDOW_WIDTH;
	if (screen_h > WINDOW_HEIGHT)
		screen_h = WINDOW_HEIGHT;
	vars->screen.width = screen_w;
	vars->screen.height = screen_h;
	usable_width = vars->screen.width - (2 * MARGIN);
	usable_height = vars->screen.height - (2 * MARGIN);
	usable_size = (usable_width < usable_height) ? usable_width : usable_height;
	max_radius = calculate_max_radius(vars);
	if (max_radius == 0)
		max_radius = 1;
	vars->screen.size = usable_size / (2 * max_radius);
	vars->screen.offset_x = vars->screen.width / 2;
	vars->screen.offset_y = vars->screen.height / 2;
}
