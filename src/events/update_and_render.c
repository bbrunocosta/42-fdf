/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_and_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:33 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

static void	update_rotation(t_vars *vars, double x_axis[3], double y_axis[3], 
							double z_axis[3], int *changed)
{
	double			step;
	t_quaternion	rot;

	(void)x_axis;
	step = 0.01;
	if (vars->keys.q)
	{
		rot = quat_from_axis_angle(z_axis[0], z_axis[1], z_axis[2], -step);
		vars->quat = quat_multiply(rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
	if (vars->keys.e)
	{
		rot = quat_from_axis_angle(z_axis[0], z_axis[1], z_axis[2], step);
		vars->quat = quat_multiply(rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
	if (vars->keys.a)
	{
		rot = quat_from_axis_angle(y_axis[0], y_axis[1], y_axis[2], -step);
		vars->quat = quat_multiply(rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
	if (vars->keys.d)
	{
		rot = quat_from_axis_angle(y_axis[0], y_axis[1], y_axis[2], step);
		vars->quat = quat_multiply(rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
}

static void	update_rotation_x(t_vars *vars, double x_axis[3], int *changed)
{
	double			step;
	t_quaternion	rot;

	step = 0.01;
	if (vars->keys.s)
	{
		rot = quat_from_axis_angle(x_axis[0], x_axis[1], x_axis[2], -step);
		vars->quat = quat_multiply(rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
	if (vars->keys.w)
	{
		rot = quat_from_axis_angle(x_axis[0], x_axis[1], x_axis[2], step);
		vars->quat = quat_multiply(rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
}

static void	update_translation(t_vars *vars, int *changed)
{
	double	move_step;

	move_step = 5.0;
	if (vars->keys.up)
	{
		vars->translation.y -= move_step;
		*changed = 1;
	}
	if (vars->keys.down)
	{
		vars->translation.y += move_step;
		*changed = 1;
	}
	if (vars->keys.left)
	{
		vars->translation.x -= move_step;
		*changed = 1;
	}
	if (vars->keys.right)
	{
		vars->translation.x += move_step;
		*changed = 1;
	}
}

static void	update_zoom(t_vars *vars, int *changed)
{
	double	zoom_step;

	zoom_step = 0.02;
	if (vars->keys.plus)
	{
		vars->zoom += zoom_step;
		if (vars->zoom > 10.0)
			vars->zoom = 10.0;
		*changed = 1;
	}
	if (vars->keys.minus)
	{
		vars->zoom -= zoom_step;
		if (vars->zoom < 0.1)
			vars->zoom = 0.1;
		*changed = 1;
	}
}

int	update_and_render(t_vars *vars)
{
	double	x_axis[3];
	double	y_axis[3];
	double	z_axis[3];
	int		changed;

	changed = 0;
	quat_to_axes(vars->quat, x_axis, y_axis, z_axis);
	update_rotation(vars, x_axis, y_axis, z_axis, &changed);
	update_rotation_x(vars, x_axis, &changed);
	update_translation(vars, &changed);
	update_zoom(vars, &changed);
	if (changed || vars->needs_render)
	{
		render(vars);
		vars->needs_render = 0;
	}
	return (0);
}
