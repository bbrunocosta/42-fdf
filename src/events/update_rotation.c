/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:13:59 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

static void	handle_z_rotation(t_vars *vars, double z_axis[3], double step,
							t_quaternion *rot)
{
	if (vars->keys.q)
	{
		*rot = quat_from_axis_angle(z_axis[0], z_axis[1], z_axis[2], -step);
		vars->quat = quat_multiply(*rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
	}
	if (vars->keys.e)
	{
		*rot = quat_from_axis_angle(z_axis[0], z_axis[1], z_axis[2], step);
		vars->quat = quat_multiply(*rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
	}
}

static void	handle_y_rotation(t_vars *vars, double y_axis[3], double step,
							t_quaternion *rot)
{
	if (vars->keys.a)
	{
		*rot = quat_from_axis_angle(y_axis[0], y_axis[1], y_axis[2], -step);
		vars->quat = quat_multiply(*rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
	}
	if (vars->keys.d)
	{
		*rot = quat_from_axis_angle(y_axis[0], y_axis[1], y_axis[2], step);
		vars->quat = quat_multiply(*rot, vars->quat);
		vars->quat = quat_normalize(vars->quat);
	}
}

void	update_rotation(t_vars *vars, double y_axis[3], double z_axis[3],
							int *changed)
{
	double			step;
	t_quaternion	rot;

	step = 0.01;
	handle_z_rotation(vars, z_axis, step, &rot);
	handle_y_rotation(vars, y_axis, step, &rot);
	if (vars->keys.a || vars->keys.d || vars->keys.q || vars->keys.e)
		*changed = 1;
}
