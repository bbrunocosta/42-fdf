/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:48:57 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

void	draw_line(t_vars *vars, t_point p1, t_point p2)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	x;
	int	y;
	int	e2;

	dx = abs((int)p2.x - (int)p1.x);
	dy = abs((int)p2.y - (int)p1.y);
	sx = p1.x < p2.x ? 1 : -1;
	sy = p1.y < p2.y ? 1 : -1;
	err = dx - dy;
	x = (int)p1.x;
	y = (int)p1.y;
	while (1)
	{
		put_pixel(vars, x, y, p1.color);
		if (x == (int)p2.x && y == (int)p2.y)
			break ;
		e2 = 2 * err;
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
