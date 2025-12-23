/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_point_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:43:16 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

int	init_point_map(t_vars *vars, unsigned int line_count)
{
	vars->point_map.points = malloc((line_count + 1) * sizeof(t_point *));
	if (!vars->point_map.points)
		return (0);
	vars->point_map.height = 0;
	vars->point_map.width = 0;
	return (1);
}
