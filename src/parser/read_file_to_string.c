/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file_to_string.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:42:09 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/23 00:28:14 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <stdlib.h>
#include <unistd.h>

char	*read_file_to_string(int fd)
{
	char	buffer[BUFFER_SIZE];
	char	*content;
	char	*temp;
	ssize_t	bytes_read;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(content, buffer);
		free(content);
		if (!temp)
			return (NULL);
		content = temp;
		bytes_read = read(fd, buffer, BUFFER_SIZE - 1);
	}
	if (bytes_read < 0)
	{
		free(content);
		return (NULL);
	}
	return (content);
}
