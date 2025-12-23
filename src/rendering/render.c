/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:56:09 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

static unsigned int	get_render_step(double zoom)
{
	if (zoom < 0.3)
		return (4);
	else if (zoom < 0.6)
		return (2);
	return (1);
}

static void	draw_horizontal(t_vars *vars, unsigned int y, unsigned int x,
				unsigned int step)
{
	t_point	p1;
	t_point	p2;

	p1 = project_iso(vars->point_map.points[y][x], vars);
	if (x + step < vars->point_map.width)
	{
		p2 = project_iso(vars->point_map.points[y][x + step], vars);
		draw_line_optimized(vars, p1, p2);
	}
}

static void	draw_vertical(t_vars *vars, unsigned int y, unsigned int x,
				unsigned int step)
{
	t_point	p1;
	t_point	p2;

	p1 = project_iso(vars->point_map.points[y][x], vars);
	if (y + step < vars->point_map.height)
	{
		p2 = project_iso(vars->point_map.points[y + step][x], vars);
		draw_line_optimized(vars, p1, p2);
	}
}

static void	render_grid(t_vars *vars, unsigned int step)
{
	unsigned int	y;
	unsigned int	x;

	y = 0;
	while (y < vars->point_map.height)
	{
		x = 0;
		while (x < vars->point_map.width)
		{
			draw_horizontal(vars, y, x, step);
			draw_vertical(vars, y, x, step);
			x += step;
		}
		y += step;
	}
}

void	render(t_vars *vars)
{
	unsigned int	step;

	ft_memset(vars->screen.addr, 0, vars->screen.height
		* vars->screen.line_length);
	step = get_render_step(vars->zoom);
	render_grid(vars, step);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->screen.img, 0, 0);
}
