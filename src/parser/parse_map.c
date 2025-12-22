/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/22 13:05:27 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int	parse_map(t_vars *vars, char *filename)
{
	int		fd;
	char	*content;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: open\n", 2);
		return (0);
	}
	content = read_file_to_string(fd);
	if (!content)
	{
		ft_putstr_fd("Error: read_file_to_string\n", 2);
		close(fd);
		return (0);
	}
	if (!try_parse(vars, content))
	{
		ft_putstr_fd("Erro: falha ao analisar o mapa\n", 2);
		free(content);
		close(fd);
		return (0);
	}
	free(content);
	close(fd);
	return (1);
}
