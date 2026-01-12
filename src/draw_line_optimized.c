/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line_optimized.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:47:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 20:09:52 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include <stdlib.h>

static void	init_line_ctx(t_line_ctx *ctx, t_point p1, t_point p2)
{
	ctx->dx = abs((int)p2.x - (int)p1.x);
	ctx->dy = abs((int)p2.y - (int)p1.y);
	ctx->sx = -1;
	ctx->sy = -1;
	if (p1.x < p2.x)
		ctx->sx = 1;
	if (p1.y < p2.y)
		ctx->sy = 1;
	ctx->err = ctx->dx - ctx->dy;
	ctx->x = (int)p1.x;
	ctx->y = (int)p1.y;
	ctx->end_x = (int)p2.x;
	ctx->end_y = (int)p2.y;
}

static void	draw_pixel(t_vars *vars, t_line_ctx *ctx, unsigned int color)
{
	int	bpp;
	int	line_len;

	bpp = vars->screen.bits_per_pixel >> 3;
	line_len = vars->screen.line_length;
	if (ctx->x >= 0 && ctx->x < (int)vars->screen.width
		&& ctx->y >= 0 && ctx->y < (int)vars->screen.height)
		*(unsigned int *)(vars->screen.addr
				+ (ctx->y * line_len + ctx->x * bpp)) = color;
}

static void	step_line(t_line_ctx *ctx)
{
	int	e2;

	e2 = ctx->err << 1;
	if (e2 > -ctx->dy)
	{
		ctx->err -= ctx->dy;
		ctx->x += ctx->sx;
	}
	if (e2 < ctx->dx)
	{
		ctx->err += ctx->dx;
		ctx->y += ctx->sy;
	}
}

void	draw_line_optimized(t_vars *vars, t_point p1, t_point p2)
{
	t_line_ctx	ctx;

	if (!is_visible(p1, vars) && !is_visible(p2, vars))
		return ;
	init_line_ctx(&ctx, p1, p2);
	while (1)
	{
		draw_pixel(vars, &ctx, p1.color.hex);
		if (ctx.x == ctx.end_x && ctx.y == ctx.end_y)
			break ;
		step_line(&ctx);
	}
}
