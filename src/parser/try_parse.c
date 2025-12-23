/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   try_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:28:26 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:36:51 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

static int	parse_point(char *token, t_point *point, unsigned int row,
			unsigned int col)
{
	char		*color_part;
	int			z;
	t_color		color;

	color_part = ft_strchr(token, ',');
	z = ft_atoi(token);
	if (color_part)
		color.hex = ft_atoi_base(color_part + 1, 16);
	else
		color.hex = 0xFFFFFF;
	point->x = col;
	point->y = row;
	point->z = z;
	point->color = color;
	return (1);
}

static int	parse_line(char *line, t_point *points, unsigned int row,
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

static void	free_lines(char **lines)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
		free(lines[i++]);
	free(lines);
}

static int	alloc_and_parse(t_vars *vars, char **lines, unsigned int row)
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

static unsigned int	count_lines(char **lines)
{
	unsigned int	i;

	i = 0;
	while (lines[i])
		i++;
	return (i);
}

static int	init_point_map(t_vars *vars, unsigned int line_count)
{
	vars->point_map.points = malloc((line_count + 1) * sizeof(t_point *));
	if (!vars->point_map.points)
		return (0);
	vars->point_map.height = 0;
	vars->point_map.width = 0;
	return (1);
}

static int	parse_all_rows(t_vars *vars, char **lines)
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
