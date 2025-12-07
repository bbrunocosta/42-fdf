/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_rotate_point.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:21:48 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:38 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

t_point	quat_rotate_point(t_quaternion q, t_point p)
{
    t_point			result;
    t_quaternion	p_quat;
    t_quaternion	q_conj;
    t_quaternion	temp;
    t_quaternion	result_quat;
	
    p_quat.w = 0;
    p_quat.x = p.x;
    p_quat.y = p.y;
    p_quat.z = p.z;
	q_conj = quat_conjugate(q);
	temp = quat_multiply(q, p_quat);
	result_quat = quat_multiply(temp, q_conj);
    result.x = result_quat.x;
    result.y = result_quat.y;
    result.z = result_quat.z;
    result.color = p.color;
    return (result);
}
