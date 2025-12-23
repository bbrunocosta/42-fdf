/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   project_iso.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:48:47 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <math.h>

static t_point	center_point(t_point p, t_vars *vars)
{
	t_point	centered;

	centered.x = p.x - (vars->point_map.width / 2.0);
	centered.y = p.y - (vars->point_map.height / 2.0);
	centered.z = p.z;
	centered.color = p.color;
	return (centered);
}

static void	apply_isometric(t_point rotated, double *iso_x, double *iso_y)
{
	double	angle;
	double	z_scale;

	angle = M_PI / 6;
	z_scale = 0.5;
	*iso_x = (rotated.x - rotated.y) * cos(angle);
	*iso_y = (rotated.x + rotated.y) * sin(angle) - (rotated.z * z_scale);
}

static t_point	screen_project(double iso_x, double iso_y, t_vars *vars,
					t_point original)
{
	t_point	projected;
	double	scale;

	scale = vars->screen.size * vars->zoom;
	projected.x = vars->screen.offset_x + iso_x * scale + vars->translation.x;
	projected.y = vars->screen.offset_y + iso_y * scale + vars->translation.y;
	projected.z = original.z;
	projected.color = original.color;
	return (projected);
}

t_point	project_iso(t_point p, t_vars *vars)
{
	t_point	centered;
	t_point	rotated;
	double	iso_x;
	double	iso_y;

	centered = center_point(p, vars);
	rotated = quat_rotate_point(vars->quat, centered);
	apply_isometric(rotated, &iso_x, &iso_y);
	return (screen_project(iso_x, iso_y, vars, rotated));
}
