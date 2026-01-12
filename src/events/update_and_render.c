/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_and_render.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 21:22:11 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	update_and_render(t_vars *vars)
{
	int	changed;

	changed = 0;
	update_rotation(vars, &changed);
	update_translation(vars, &changed);
	update_zoom(vars, &changed);
	if (changed || vars->needs_render)
	{
		render(vars);
		vars->needs_render = 0;
	}
	return (0);
}
