/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 19:15:20 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include "mlx.h"
#include <math.h>

static t_point	get_rotated_pos(t_vars *vars, int x, int y)
{
	t_point			centered;
	t_quaternion	quat_inv;

	centered.x = x - (vars->point_map.width - 1) / 2.0;
	centered.y = y - (vars->point_map.height - 1) / 2.0;
	centered.z = vars->point_map.points[y][x].z;
	quat_inv = quat_conjugate(vars->quat);
	return (quat_rotate_point(quat_inv, centered));
}

void	render(t_vars *vars)
{
	int				y;
	int				x;
	t_point			p1;
	t_point			p2;
	int				start_y, end_y, step_y;
	int				start_x, end_x, step_x;
	t_point			corner_top;
	t_point			corner_bottom;
	t_point			corner_left;
	t_point			corner_right;

	ft_memset(vars->screen.addr, 0, vars->screen.height
		* vars->screen.line_length);
	corner_top = get_rotated_pos(vars, vars->point_map.width / 2, 0);
	corner_bottom = get_rotated_pos(vars, vars->point_map.width / 2, vars->point_map.height - 1);
	corner_left = get_rotated_pos(vars, 0, vars->point_map.height / 2);
	corner_right = get_rotated_pos(vars, vars->point_map.width - 1, vars->point_map.height / 2);
	if (corner_top.z > corner_bottom.z)
	{
		start_y = vars->point_map.height - 1;
		end_y = -1;
		step_y = -1;
	}
	else
	{
		start_y = 0;
		end_y = vars->point_map.height;
		step_y = 1;
	}
	if (corner_left.z > corner_right.z)
	{
		start_x = vars->point_map.width - 1;
		end_x = -1;
		step_x = -1;
	}
	else
	{
		start_x = 0;
		end_x = vars->point_map.width;
		step_x = 1;
	}
	y = start_y;
	while (y != end_y)
	{
		x = start_x;
		while (x != end_x)
		{
			p1 = project_iso(vars->point_map.points[y][x], vars);
			if ((x + step_x >= 0) && (x + step_x < (int)vars->point_map.width))
			{
				p2 = project_iso(vars->point_map.points[y][x + step_x], vars);
				draw_line_optimized(vars, p1, p2);
			}
			if ((y + step_y >= 0) && (y + step_y < (int)vars->point_map.height))
			{
				p2 = project_iso(vars->point_map.points[y + step_y][x], vars);
				draw_line_optimized(vars, p1, p2);
			}
			x += step_x;
		}
		y += step_y;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->screen.img, 0, 0);
}
