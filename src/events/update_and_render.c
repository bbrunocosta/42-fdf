/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_and_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 17:43:20 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	update_and_render(t_vars *vars)
{
	int				changed;
	t_quaternion	qx;
	t_quaternion	qy;
	t_quaternion	qz;
	t_quaternion	temp;

	changed = 0;
	update_rotation(vars, &changed);
	update_rotation_x(vars, &changed);
	update_translation(vars, &changed);
	update_zoom(vars, &changed);
	if (changed || vars->needs_render)
	{
		qx = quat_from_axis_angle(1, 0, 0, vars->rotation.x);
		qy = quat_from_axis_angle(0, 1, 0, vars->rotation.y);
		qz = quat_from_axis_angle(0, 0, 1, vars->rotation.z);
		temp = quat_multiply(qy, qx);
		vars->quat = quat_multiply(qz, temp);
		vars->quat = quat_normalize(vars->quat);
		render(vars);
		vars->needs_render = 0;
	}
	return (0);
}
