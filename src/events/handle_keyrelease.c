/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keyrelease.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:50:03 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"

int	handle_keyrelease(int keycode, t_vars *vars)
{
	if (keycode == KEY_W)
		vars->keys.w = 0;
	else if (keycode == KEY_S)
		vars->keys.s = 0;
	else if (keycode == KEY_A)
		vars->keys.a = 0;
	else if (keycode == KEY_D)
		vars->keys.d = 0;
	else if (keycode == KEY_Q)
		vars->keys.q = 0;
	else if (keycode == KEY_E)
		vars->keys.e = 0;
	else if (keycode == KEY_UP)
		vars->keys.up = 0;
	else if (keycode == KEY_DOWN)
		vars->keys.down = 0;
	else if (keycode == KEY_LEFT)
		vars->keys.left = 0;
	else if (keycode == KEY_RIGHT)
		vars->keys.right = 0;
	else if (keycode == KEY_PLUS)
		vars->keys.plus = 0;
	else if (keycode == KEY_MINUS)
		vars->keys.minus = 0;
	return (0);
}
