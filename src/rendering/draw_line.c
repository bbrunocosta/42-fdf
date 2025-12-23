/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:55:12 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

static void	init_bresenham(t_bresenham *b, t_point p1, t_point p2)
{
	b->dx = abs((int)p2.x - (int)p1.x);
	b->dy = abs((int)p2.y - (int)p1.y);
	if (p1.x < p2.x)
		b->sx = 1;
	else
		b->sx = -1;
	if (p1.y < p2.y)
		b->sy = 1;
	else
		b->sy = -1;
	b->err = b->dx - b->dy;
}

static void	bresenham_step(t_bresenham *b, int *x, int *y)
{
	int	e2;

	e2 = 2 * b->err;
	if (e2 > -b->dy)
	{
		b->err -= b->dy;
		*x += b->sx;
	}
	if (e2 < b->dx)
	{
		b->err += b->dx;
		*y += b->sy;
	}
}

void	draw_line(t_vars *vars, t_point p1, t_point p2)
{
	t_bresenham	b;
	int			x;
	int			y;

	init_bresenham(&b, p1, p2);
	x = (int)p1.x;
	y = (int)p1.y;
	while (1)
	{
		put_pixel(vars, x, y, p1.color);
		if (x == (int)p2.x && y == (int)p2.y)
			break ;
		bresenham_step(&b, &x, &y);
	}
}
