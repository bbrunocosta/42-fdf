/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/05 17:43:19 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_rotation(t_vars *vars, int *changed)
{
	double	step;

	step = 0.05;
	if (vars->keys.q)
	{
		vars->rotation.z += step;
		*changed = 1;
	}
	if (vars->keys.e)
	{
		vars->rotation.z -= step;
		*changed = 1;
	}
	if (vars->keys.a)
	{
		vars->rotation.y += step;
		*changed = 1;
	}
	if (vars->keys.d)
	{
		vars->rotation.y -= step;
		*changed = 1;
	}
}
