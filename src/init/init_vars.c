/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 23:33:17 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	init_vars(t_vars *vars)
{
	t_quaternion	qx;
	t_quaternion	qy;

	vars->quat = quat_identity();
	qy = quat_from_axis_angle(1, 0, 0, -45.0 * DEG_TO_RAD);
	vars->quat = quat_multiply(qy, vars->quat);
	qx = quat_from_axis_angle(0, 1, 0, 35 * DEG_TO_RAD);
	vars->quat = quat_multiply(qx, vars->quat);
	vars->quat = quat_normalize(vars->quat);
	vars->translation.x = 0;
	vars->translation.y = 0;
	vars->zoom = 1.0;
	vars->needs_render = 1;
	ft_memset(&vars->keys, 0, sizeof(t_keys));
}
