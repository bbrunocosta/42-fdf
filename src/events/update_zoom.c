/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_zoom.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 12:57:47 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_zoom(t_vars *vars, int *changed)
{
	double	zoom_step;

	zoom_step = 0.02;
	if (vars->keys.plus)
	{
		vars->zoom += zoom_step;
		if (vars->zoom > 10.0)
			vars->zoom = 10.0;
		*changed = 1;
	}
	if (vars->keys.minus)
	{
		vars->zoom -= zoom_step;
		if (vars->zoom < 0.001)
			vars->zoom = 0.001;
		*changed = 1;
	}
}
