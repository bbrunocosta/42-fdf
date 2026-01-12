/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation_y.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 20:38:55 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_rotation_y(t_vars *vars, int *changed, long current_time)
{
	if (vars->keys.a
		&& (current_time - vars->last_rotation_y_time) >= ROTATION_DEBOUNCE_MS)
	{
		vars->rotation.y += ROTATION_STEP;
		vars->last_rotation_y_time = current_time;
		*changed = 1;
	}
	if (vars->keys.d
		&& (current_time - vars->last_rotation_y_time) >= ROTATION_DEBOUNCE_MS)
	{
		vars->rotation.y -= ROTATION_STEP;
		vars->last_rotation_y_time = current_time;
		*changed = 1;
	}
}
