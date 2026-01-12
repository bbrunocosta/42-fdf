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

static void	init_render_y(t_vars *vars, t_render_ctx *ctx)
{
	t_point	corner_top;
	t_point	corner_bottom;

	corner_top = get_rotated_pos(vars, vars->point_map.width / 2, 0);
	corner_bottom = get_rotated_pos(vars, vars->point_map.width / 2,
			vars->point_map.height - 1);
	if (corner_top.z > corner_bottom.z)
	{
		ctx->start_y = vars->point_map.height - 1;
		ctx->end_y = -1;
		ctx->step_y = -1;
	}
	else
	{
		ctx->start_y = 0;
		ctx->end_y = vars->point_map.height;
		ctx->step_y = 1;
	}
}

static void	init_render_x(t_vars *vars, t_render_ctx *ctx)
{
	t_point	corner_left;
	t_point	corner_right;

	corner_left = get_rotated_pos(vars, 0, vars->point_map.height / 2);
	corner_right = get_rotated_pos(vars, vars->point_map.width - 1,
			vars->point_map.height / 2);
	if (corner_left.z > corner_right.z)
	{
		ctx->start_x = vars->point_map.width - 1;
		ctx->end_x = -1;
		ctx->step_x = -1;
	}
	else
	{
		ctx->start_x = 0;
		ctx->end_x = vars->point_map.width;
		ctx->step_x = 1;
	}
}

static void	render_row(t_vars *vars, t_render_ctx *ctx, int y)
{
	int		x;
	t_point	p1;
	t_point	p2;

	x = ctx->start_x;
	while (x != ctx->end_x)
	{
		p1 = project_iso(vars->point_map.points[y][x], vars);
		if ((x + ctx->step_x >= 0)
			&& (x + ctx->step_x < (int)vars->point_map.width))
		{
			p2 = project_iso(vars->point_map.points[y][x + ctx->step_x], vars);
			draw_line_optimized(vars, p1, p2);
		}
		if ((y + ctx->step_y >= 0)
			&& (y + ctx->step_y < (int)vars->point_map.height))
		{
			p2 = project_iso(vars->point_map.points[y + ctx->step_y][x], vars);
			draw_line_optimized(vars, p1, p2);
		}
		x += ctx->step_x;
	}
}

void	render(t_vars *vars)
{
	t_render_ctx	ctx;
	int				y;

	ft_memset(vars->screen.addr, 0, vars->screen.height
		* vars->screen.line_length);
	init_render_y(vars, &ctx);
	init_render_x(vars, &ctx);
	y = ctx.start_y;
	while (y != ctx.end_y)
	{
		render_row(vars, &ctx, y);
		y += ctx.step_y;
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->screen.img, 0, 0);
}
