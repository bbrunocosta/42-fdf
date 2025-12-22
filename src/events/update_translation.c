/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_translation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 12:57:46 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	update_translation(t_vars *vars, int *changed)
{
	double	move_step;

	move_step = 5.0;
	if (vars->keys.up)
	{
		vars->translation.y -= move_step;
		*changed = 1;
	}
	if (vars->keys.down)
	{
		vars->translation.y += move_step;
		*changed = 1;
	}
	if (vars->keys.left)
	{
		vars->translation.x -= move_step;
		*changed = 1;
	}
	if (vars->keys.right)
	{
		vars->translation.x += move_step;
		*changed = 1;
	}
}
