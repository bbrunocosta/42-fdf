/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:33:27 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static int	print_fd_error(void)
{
	ft_putstr_fd("Error: open\n", 2);
	return (0);
}

static int	print_readfile_error(int fd)
{
	ft_putstr_fd("Error: read_file_to_string\n", 2);
	close(fd);
	return (0);
}

static int	print_parse_error(char *content, int fd)
{
	ft_putstr_fd("Error: failed to parse map\n", 2);
	free(content);
	close(fd);
	return (0);
}

int	parse_map(t_vars *vars, char *filename)
{
	int		fd;
	char	*content;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (print_fd_error());
	content = read_file_to_string(fd);
	if (!content)
		return (print_readfile_error(fd));
	if (!try_parse(vars, content))
		return (print_parse_error(content, fd));
	free(content);
	close(fd);
	return (1);
}
