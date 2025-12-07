/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file_to_string.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:42:09 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/12/06 21:55:36 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../fdf.h"
#include <string.h>
#include <stdlib.h>

char	*read_file_to_string(int fd)
{
    char	buffer[BUFFER_SIZE];
    char	*content;
	size_t	total_size;
	ssize_t	bytesRead;
	char	*new_content;

	content = malloc(1);
    if (!content)
        return NULL;
    content[0] = '\0';
	total_size = 0;
    while ((bytesRead = read(fd, buffer, BUFFER_SIZE - 1)) > 0)
    {
        buffer[bytesRead] = '\0';
        total_size += bytesRead;
        
        new_content = realloc(content, total_size + 1);
        if (!new_content)
        {
            free(content);
            return NULL;
        }
        content = new_content;
        strcat(content, buffer);
    }
    
    if (bytesRead < 0) {
        free(content);
        return NULL;
    }

    return content;
}
