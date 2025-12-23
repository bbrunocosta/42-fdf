/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_all_rows.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:43:17 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	parse_all_rows(t_vars *vars, char **lines)
{
	unsigned int	row;

	row = 0;
	while (lines[row])
	{
		if (!alloc_and_parse(vars, lines, row))
			return (0);
		row++;
	}
	vars->point_map.points[vars->point_map.height] = NULL;
	return (1);
}
