/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_from_axis_angle.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:32:31 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:37 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <math.h>

t_quaternion	quat_from_axis_angle(double axis_x, double axis_y,
		double axis_z, double angle)
{
	t_quaternion	q;
	double			half_angle;
	double			sin_half;
	double			len;

	half_angle = angle / 2.0;
	sin_half = sin(half_angle);
	len = sqrt(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
	if (len < 0.0001)
		return (quat_identity());
	axis_x /= len;
	axis_y /= len;
	axis_z /= len;
	q.w = cos(half_angle);
	q.x = axis_x * sin_half;
	q.y = axis_y * sin_half;
	q.z = axis_z * sin_half;
	return (q);
}
