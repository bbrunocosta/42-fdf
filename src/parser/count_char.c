/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:35:25 by bcosta-b          #+#    #+#             */
/*   Updated: 2025/11/20 19:35:44 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	count_char(const char *str, char c)
{
    int	count;

	count = 0;
    while (*str) {
        if (*str == c)
            count++;
        str++;
    }
    return (count);
}
