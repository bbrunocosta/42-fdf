/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:43:33 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	parse_point(char *token, t_point *point, unsigned int row,
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
