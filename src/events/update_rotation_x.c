/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation_x.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 18:12:20 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

static long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	update_rotation_x(t_vars *vars, int *changed)
{
	double	step;
	long	current_time;

	step = ROTATION_STEP;
	current_time = get_time_ms();
	if (vars->keys.s && (current_time - vars->last_rotation_x_time) >= ROTATION_DEBOUNCE_MS)
	{
		vars->rotation.x += step;
		vars->last_rotation_x_time = current_time;
		*changed = 1;
	}
	if (vars->keys.w && (current_time - vars->last_rotation_x_time) >= ROTATION_DEBOUNCE_MS)
	{
		vars->rotation.x -= step;
		vars->last_rotation_x_time = current_time;
		*changed = 1;
	}
}
