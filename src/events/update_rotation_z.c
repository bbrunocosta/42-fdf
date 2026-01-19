/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation_z.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/18 22:19:01 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_rotation_z(t_vars *vars, int *changed)
{
	t_quaternion	delta;
	double			rot_step;

	rot_step = 5.0 * (M_PI / 180.0);
	if (vars->keys.q)
	{
		delta = quat_from_axis_angle(0, 0, 1, rot_step);
		vars->quat = quat_multiply(delta, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
	if (vars->keys.e)
	{
		delta = quat_from_axis_angle(0, 0, 1, -rot_step);
		vars->quat = quat_multiply(delta, vars->quat);
		vars->quat = quat_normalize(vars->quat);
		*changed = 1;
	}
}
