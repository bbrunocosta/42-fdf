/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation_x.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 12:57:46 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_rotation_x(t_vars *vars, double x_axis[3], int *changed)
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
