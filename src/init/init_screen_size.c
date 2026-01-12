/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_screen_size.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 19:58:14 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

static double	get_dist(t_vars *vars, t_radius_ctx *ctx,
		unsigned int x, unsigned int y)
{
	double	dx;
	double	dy;
	double	dz;

	dx = x - ctx->center_x;
	dy = y - ctx->center_y;
	dz = vars->point_map.points[y][x].z;
	return (sqrt(dx * dx + dy * dy + dz * dz));
}

static double	calculate_max_radius(t_vars *vars)
{
	t_radius_ctx	ctx;
	unsigned int	y;
	unsigned int	x;
	double			dist;

	ctx.center_x = (vars->point_map.width - 1) / 2.0;
	ctx.center_y = (vars->point_map.height - 1) / 2.0;
	ctx.max_radius = 0;
	y = 0;
	while (y < vars->point_map.height)
	{
		x = 0;
		while (x < vars->point_map.width)
		{
			dist = get_dist(vars, &ctx, x, y);
			if (dist > ctx.max_radius)
				ctx.max_radius = dist;
			x++;
		}
		y++;
	}
	return (ctx.max_radius);
}

static void	init_screen_dimensions(t_vars *vars)
{
	int	screen_w;
	int	screen_h;

	mlx_get_screen_size(vars->mlx, &screen_w, &screen_h);
	if (screen_w > WINDOW_WIDTH)
		screen_w = WINDOW_WIDTH;
	if (screen_h > WINDOW_HEIGHT)
		screen_h = WINDOW_HEIGHT;
	vars->screen.width = screen_w;
	vars->screen.height = screen_h;
}

void	init_screen_size(t_vars *vars)
{
	double	usable_size;
	double	max_radius;

	init_screen_dimensions(vars);
	usable_size = vars->screen.width - (2 * MARGIN);
	if (vars->screen.height - (2 * MARGIN) < usable_size)
		usable_size = vars->screen.height - (2 * MARGIN);
	max_radius = calculate_max_radius(vars);
	if (max_radius == 0)
		max_radius = 1;
	vars->screen.size = usable_size / (2 * max_radius);
	vars->screen.offset_x = vars->screen.width / 2;
	vars->screen.offset_y = vars->screen.height / 2;
}
