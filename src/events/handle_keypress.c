/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keypress.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 22:57:38 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

static void	apply_rotation(t_vars *vars, double x, double y, double z, double a)
{
	t_quaternion	delta;

	delta = quat_from_axis_angle(x, y, z, a);
	vars->quat = quat_multiply(delta, vars->quat);
	vars->quat = quat_normalize(vars->quat);
	vars->needs_render = 1;
}

static void	handle_movement_keys(int keycode, t_vars *vars)
{
	if (keycode == KEY_W)
		apply_rotation(vars, 1, 0, 0, ROTATION_STEP);
	else if (keycode == KEY_S)
		apply_rotation(vars, 1, 0, 0, -ROTATION_STEP);
	else if (keycode == KEY_A)
		apply_rotation(vars, 0, 0, 1, -ROTATION_STEP);
	else if (keycode == KEY_D)
		apply_rotation(vars, 0, 0, 1, ROTATION_STEP);
	else if (keycode == KEY_Q)
		apply_rotation(vars, 0, 1, 0, -ROTATION_STEP);
	else if (keycode == KEY_E)
		apply_rotation(vars, 0, 1, 0, ROTATION_STEP);
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
		close_window(vars);
	if (keycode == KEY_P)
	{
		vars->quat = quat_identity();
		vars->needs_render = 1;
	}
	handle_movement_keys(keycode, vars);
	handle_arrow_keys(keycode, vars);
	return (0);
}
