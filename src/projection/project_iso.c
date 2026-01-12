/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   project_iso.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 17:24:47 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <math.h>

t_point	project_iso(t_point p, t_vars *vars)
{
	t_point			projected;
	t_point			centered;
	t_point			rotated;
	double			scale;
	t_quaternion	quat_inv;

	centered.x = p.x - (vars->point_map.width - 1) / 2.0;
	centered.y = p.y - (vars->point_map.height - 1) / 2.0;
	centered.z = p.z;
	centered.color = p.color;
	quat_inv = quat_conjugate(vars->quat);
	rotated = quat_rotate_point(quat_inv, centered);
	scale = vars->screen.size * vars->zoom;
	projected.x = vars->screen.offset_x + rotated.x
		* scale + vars->translation.x;
	projected.y = vars->screen.offset_y + rotated.y
		* scale + vars->translation.y;
	projected.z = rotated.z;
	projected.color = p.color;
	return (projected);
}
