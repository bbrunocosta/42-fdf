/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line_optimized.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:47:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:35 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdlib.h>

static inline int is_visible(t_point p, t_vars *vars)
{
    int margin = 100;
    return (p.x >= -margin && p.x < vars->screen.width + margin &&
            p.y >= -margin && p.y < vars->screen.height + margin);
}

void draw_line_optimized(t_vars *vars, t_point p1, t_point p2)
{
	int dx;
	int dy;
	int sx;
	int sy;
	int err;
	int x;
	int y;
	int end_x;
	int end_y;
	unsigned int color;
	int bpp;
	int line_len;
	int max_x;
	int max_y;
	int e2;

	if (!is_visible(p1, vars) && !is_visible(p2, vars))
		return;
	dx = abs((int)p2.x - (int)p1.x);
	dy = abs((int)p2.y - (int)p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;
	err = dx - dy;
	x = (int)p1.x;
	y = (int)p1.y;
	end_x = (int)p2.x;
	end_y = (int)p2.y;
	color = p1.color.hex;
	bpp = vars->screen.bits_per_pixel >> 3;
	line_len = vars->screen.line_length;
	max_x = (int)vars->screen.width;
	max_y = (int)vars->screen.height;
	while (1)
	{
		if (x >= 0 && x < max_x && y >= 0 && y < max_y)
			*(unsigned int*)(vars->screen.addr + (y * line_len + x * bpp)) = color;
		if (x == end_x && y == end_y)
			break;
		e2 = err << 1;
		if (e2 > -dy)
		{
			err -= dy;
			x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y += sy;
		}
	}
}