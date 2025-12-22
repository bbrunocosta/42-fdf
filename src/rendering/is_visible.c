/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_visible.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 12:57:44 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	is_visible(t_point p, t_vars *vars)
{
	int	margin;

	margin = 100;
	return (p.x >= -margin && p.x < vars->screen.width + margin
		&& p.y >= -margin && p.y < vars->screen.height + margin);
}
