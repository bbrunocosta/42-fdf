/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_points.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:37:11 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:36 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "fdf.h"

int	free_points(t_vars *vars)
{
	unsigned int	x;

	if (!vars || !vars->point_map.points)
		return (0);
	x = 0;
	while (vars->point_map.points[x])
	{
		free(vars->point_map.points[x]);
		x++;
	}
	free(vars->point_map.points);
	return (1);
}
