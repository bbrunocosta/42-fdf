/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 22:53:53 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"
#include "mlx.h"

int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc != 2)
	{
		ft_putstr_fd("usage: ./fdf <file.fdf>\n", 2);
		return (1);
	}
	ft_memset(&vars, 0, sizeof(t_vars));
	if (!parse_map(&vars, argv[1]))
		return (1);
	if (!setup_mlx(&vars))
	{
		free_points(&vars);
		return (1);
	}
	init_vars(&vars);
	mlx_hook(vars.win, 17, 0, close_window, &vars);
	mlx_hook(vars.win, 2, 1L << 0, handle_keypress, &vars);
	mlx_hook(vars.win, 3, 1L << 1, handle_keyrelease, &vars);
	mlx_loop_hook(vars.mlx, update_and_render, &vars);
	render(&vars);
	mlx_loop(vars.mlx);
	return (0);
}
