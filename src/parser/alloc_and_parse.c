/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:43:49 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

int	alloc_and_parse(t_vars *vars, char **lines, unsigned int row)
{
	unsigned int	cols;

	cols = count_tokens(lines[row]);
	if (cols == 0)
		return (1);
	if (vars->point_map.width == 0)
		vars->point_map.width = cols;
	else if (vars->point_map.width != cols)
		return (0);
	vars->point_map.points[vars->point_map.height] = malloc(cols
			* sizeof(t_point));
	if (!vars->point_map.points[vars->point_map.height])
		return (0);
	if (!parse_line(lines[row], vars->point_map.points[vars->point_map.height],
			vars->point_map.height, cols))
		return (0);
	vars->point_map.height++;
	return (1);
}
