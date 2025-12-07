/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:48:59 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

void	put_pixel(t_vars *vars, int x, int y, t_color color)
{
	char	*dst;

	if (x < 0 || x >= vars->screen.width || y < 0 || y >= vars->screen.height)
		return ;
	dst = vars->screen.addr + (y * vars->screen.line_length + x * (vars->screen.bits_per_pixel / 8));
	*(unsigned int *)dst = color.hex;
}
