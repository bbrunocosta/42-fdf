/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:43:27 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

int	parse_line(char *line, t_point *points, unsigned int row,
		unsigned int expected_cols)
{
	char			**tokens;
	unsigned int	col;
	unsigned int	i;

	tokens = ft_split(line, ' ');
	if (!tokens)
		return (0);
	col = 0;
	while (tokens[col] && col < expected_cols)
	{
		if (!parse_point(tokens[col], &points[col], row, col))
		{
			i = 0;
			while (tokens[i])
				free(tokens[i++]);
			free(tokens);
			return (0);
		}
		free(tokens[col]);
		col++;
	}
	free(tokens);
	return (col == expected_cols);
}
