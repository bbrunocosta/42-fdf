/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 20:38:54 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

static long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	update_rotation(t_vars *vars, int *changed)
{
	long	current_time;

	current_time = get_time_ms();
	update_rotation_z(vars, changed, current_time);
	update_rotation_y(vars, changed, current_time);
}
