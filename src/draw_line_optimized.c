/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line_optimized.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:47:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 01:01:29 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdlib.h>

static void	init_line_params(t_bresenham *b, t_point p1, t_point p2)
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

static void	put_pixel_fast(t_vars *vars, int x, int y, unsigned int color)
{
	int	offset;
	int	bpp;

	if (x >= 0 && x < (int)vars->screen.width && y >= 0
		&& y < (int)vars->screen.height)
	{
		bpp = vars->screen.bits_per_pixel >> 3;
		offset = y * vars->screen.line_length + x * bpp;
		*(unsigned int *)(vars->screen.addr + offset) = color;
	}
}

static void	line_step(t_bresenham *b, int *x, int *y)
{
	int	e2;

	e2 = b->err << 1;
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

void	draw_line_optimized(t_vars *vars, t_point p1, t_point p2)
{
	t_bresenham		b;
	int				x;
	int				y;
	int				end_x;
	int				end_y;

	if (!is_visible(p1, vars) && !is_visible(p2, vars))
		return ;
	init_line_params(&b, p1, p2);
	x = (int)p1.x;
	y = (int)p1.y;
	end_x = (int)p2.x;
	end_y = (int)p2.y;
	while (1)
	{
		put_pixel_fast(vars, x, y, p1.color.hex);
		if (x == end_x && y == end_y)
			break ;
		line_step(&b, &x, &y);
	}
}
