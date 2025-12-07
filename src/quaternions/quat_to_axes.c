/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_to_axes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:24:28 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:38 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	quat_to_axes(t_quaternion q, double *x_axis, double *y_axis, double *z_axis)
{
    x_axis[0] = 1 - 2 * (q.y * q.y + q.z * q.z);
    x_axis[1] = 2 * (q.x * q.y + q.w * q.z);
    x_axis[2] = 2 * (q.x * q.z - q.w * q.y);
    
    y_axis[0] = 2 * (q.x * q.y - q.w * q.z);
    y_axis[1] = 1 - 2 * (q.x * q.x + q.z * q.z);
    y_axis[2] = 2 * (q.y * q.z + q.w * q.x);
    
    z_axis[0] = 2 * (q.x * q.z + q.w * q.y);
    z_axis[1] = 2 * (q.y * q.z - q.w * q.x);
    z_axis[2] = 1 - 2 * (q.x * q.x + q.y * q.y);
}
