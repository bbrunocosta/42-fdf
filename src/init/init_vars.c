/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 18:12:31 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	init_vars(t_vars *vars)
{
	vars->quat = quat_identity();
	vars->rotation.x = 0;
	vars->rotation.y = 0;
	vars->rotation.z = 0;
	vars->translation.x = 0;
	vars->translation.y = 0;
	vars->zoom = 1.0;
	vars->needs_render = 1;
	vars->last_rotation_x_time = 0;
	vars->last_rotation_y_time = 0;
	vars->last_rotation_z_time = 0;
	ft_memset(&vars->keys, 0, sizeof(t_keys));
}
