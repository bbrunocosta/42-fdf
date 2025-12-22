/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 13:17:29 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

void	render(t_vars *vars)
{
	unsigned int	step;
	unsigned int	y;
	unsigned int	x;
	t_point			p1;
	t_point			p2;

	ft_memset(vars->screen.addr, 0, vars->screen.height
		* vars->screen.line_length);
	step = 1;
	if (vars->zoom < 0.3)
		step = 4;
	else if (vars->zoom < 0.6)
		step = 2;
	y = 0;
	while (y < vars->point_map.height)
	{
		x = 0;
		while (x < vars->point_map.width)
		{
			p1 = project_iso(vars->point_map.points[y][x], vars);
			if (x + step < vars->point_map.width)
			{
				p2 = project_iso(vars->point_map.points[y][x + step], vars);
				draw_line_optimized(vars, p1, p2);
			}
			if (y + step < vars->point_map.height)
			{
				p2 = project_iso(vars->point_map.points[y + step][x], vars);
				draw_line_optimized(vars, p1, p2);
			}
			x += step;
		}
		y += step;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->screen.img, 0, 0);
}
