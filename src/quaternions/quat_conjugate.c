/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quat_conjugate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:29:09 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:37 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

t_quaternion	quat_conjugate(t_quaternion q)
{
    t_quaternion	conj;

    conj.w = q.w;
    conj.x = -q.x;
    conj.y = -q.y;
    conj.z = -q.z;
    return (conj);
}
