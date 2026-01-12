/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keypress.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 20:03:26 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

static void	handle_movement_keys(int keycode, t_vars *vars)
{
	if (keycode == KEY_W)
		vars->keys.w = 1;
	else if (keycode == KEY_S)
		vars->keys.s = 1;
	else if (keycode == KEY_A)
		vars->keys.a = 1;
	else if (keycode == KEY_D)
		vars->keys.d = 1;
	else if (keycode == KEY_Q)
		vars->keys.q = 1;
	else if (keycode == KEY_E)
		vars->keys.e = 1;
}

static void	handle_arrow_keys(int keycode, t_vars *vars)
{
	if (keycode == KEY_UP)
		vars->keys.up = 1;
	else if (keycode == KEY_DOWN)
		vars->keys.down = 1;
	else if (keycode == KEY_LEFT)
		vars->keys.left = 1;
	else if (keycode == KEY_RIGHT)
		vars->keys.right = 1;
	else if (keycode == KEY_PLUS)
		vars->keys.plus = 1;
	else if (keycode == KEY_MINUS)
		vars->keys.minus = 1;
}

int	handle_keypress(int keycode, t_vars *vars)
{
	if (keycode == KEY_ESC)
		exit(0);
	handle_movement_keys(keycode, vars);
	handle_arrow_keys(keycode, vars);
	return (0);
}
