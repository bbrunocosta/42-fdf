/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:28:26 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:43:54 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

int	try_parse(t_vars *vars, char *map)
{
	char			**lines;
	unsigned int	line_count;

	if (!vars || !map)
		return (0);
	lines = ft_split(map, '\n');
	if (!lines)
		return (0);
	line_count = count_lines(lines);
	if (line_count == 0 || !init_point_map(vars, line_count))
		return (free_lines(lines), 0);
	if (!parse_all_rows(vars, lines))
		return (free_points(vars), free_lines(lines), 0);
	return (free_lines(lines), 1);
}
