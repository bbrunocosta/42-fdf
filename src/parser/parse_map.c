/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 19:35:56 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static int	put_error(char *message)
{
	ft_putstr_fd(message, 2);
	return (0);
}

int	parse_map(t_vars *vars, char *filename)
{
	int		fd;
	char	*content;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (put_error("Open file error\n"));
	content = read_file_to_string(fd);
	if (!content)
	{
		ft_putstr_fd("Read file error\n", 2);
		close(fd);
		return (0);
	}
	if (!try_parse(vars, content))
	{
		ft_putstr_fd("Map error\n", 2);
		free(content);
		close(fd);
		return (0);
	}
	free(content);
	close(fd);
	return (1);
}
