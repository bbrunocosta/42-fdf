/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcosta-b <bcosta-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:36:03 by bcosta-b          #+#    #+#             */
/*   Updated: 2026/01/11 19:36:14 by bcosta-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	count_tokens(const char *str)
{
	int	count;
	int	in_token;

	count = 0;
	in_token = 0;
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
		{
			if (!in_token)
			{
				count++;
				in_token = 1;
			}
		}
		else
		{
			in_token = 0;
		}
		str++;
	}
	return (count);
}
