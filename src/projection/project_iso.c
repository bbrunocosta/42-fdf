/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   project_iso.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:50:03 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <math.h>

t_point	project_iso(t_point p, t_vars *vars)
{
	t_point	projected;
	t_point	centered;
	t_point	rotated;
	double	angle;
	double	z_scale;
	double	iso_x;
	double	iso_y;
	double	scale;

	angle = M_PI / 6;
	z_scale = 0.5;
	centered.x = p.x - (vars->point_map.width / 2.0);
	centered.y = p.y - (vars->point_map.height / 2.0);
	centered.z = p.z;
	centered.color = p.color;
	rotated = quat_rotate_point(vars->quat, centered);
	iso_x = (rotated.x - rotated.y) * cos(angle);
	iso_y = (rotated.x + rotated.y) * sin(angle) - (rotated.z * z_scale);
	scale = vars->screen.size * vars->zoom;
	projected.x = vars->screen.offset_x + iso_x * scale + vars->translation.x;
	projected.y = vars->screen.offset_y + iso_y * scale + vars->translation.y;
	projected.z = rotated.z;
	projected.color = p.color;
	return (projected);
}
