/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_normalize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:30:21 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:37 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <math.h>

t_quaternion	quat_normalize(t_quaternion q)
{
    double	len;

	len = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    if (len < 0.0001)
        return (quat_identity());
    q.w /= len;
    q.x /= len;
    q.y /= len;
    q.z /= len;
    return (q);
}
