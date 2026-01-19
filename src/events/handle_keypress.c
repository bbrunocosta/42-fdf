/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_keypress.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/18 22:18:42 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>

static void	apply_rotation(t_vars *vars, double config[4])
{
	t_quaternion	delta;

	delta = quat_from_axis_angle(config[0], config[1], config[2], config[3]);
	vars->quat = quat_multiply(delta, vars->quat);
	vars->quat = quat_normalize(vars->quat);
	vars->needs_render = 1;
}

static void	handle_movement_keys(int keycode, t_vars *vars)
{
	double			rot_step;

	rot_step = 5.0 * (M_PI / 180.0);
	if (keycode == KEY_W)
		apply_rotation(vars, (double [4]){1, 0, 0, rot_step});
	else if (keycode == KEY_S)
		apply_rotation(vars, (double [4]){1, 0, 0, -rot_step});
	else if (keycode == KEY_A)
		apply_rotation(vars, (double [4]){0, 0, 1, -rot_step});
	else if (keycode == KEY_D)
		apply_rotation(vars, (double [4]){0, 0, 1, rot_step});
	else if (keycode == KEY_Q)
		apply_rotation(vars, (double [4]){0, 1, 0, -rot_step});
	else if (keycode == KEY_E)
		apply_rotation(vars, (double [4]){0, 1, 0, rot_step});
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
