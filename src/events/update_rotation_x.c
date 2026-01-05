/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation_x.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 17:43:19 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_rotation_x(t_vars *vars, int *changed)
{
	double	step;

	step = 0.05;
	if (vars->keys.s)
	{
		vars->rotation.x += step;
		*changed = 1;
	}
	if (vars->keys.w)
	{
		vars->rotation.x -= step;
		*changed = 1;
	}
}
