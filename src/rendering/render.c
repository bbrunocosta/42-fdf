/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 11:57:28 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"

void	render(t_vars *vars)
{
	unsigned int	y;
	unsigned int	x;
	t_point			p1;
	t_point			p2;

	ft_memset(vars->screen.addr, 0, vars->screen.height
		* vars->screen.line_length);
	y = 0;
	while (y < vars->point_map.height)
	{
		x = 0;
		while (x < vars->point_map.width)
		{
			p1 = project_iso(vars->point_map.points[y][x], vars);
			if (x + 1 < vars->point_map.width)
			{
				p2 = project_iso(vars->point_map.points[y][x + 1], vars);
				draw_line_optimized(vars, p1, p2);
			}
			if (y + 1 < vars->point_map.height)
			{
				p2 = project_iso(vars->point_map.points[y + 1][x], vars);
				draw_line_optimized(vars, p1, p2);
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->screen.img, 0, 0);
}
