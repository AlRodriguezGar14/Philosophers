/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:14:11 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/09 21:06:41 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*char_dest;
	const unsigned char	*char_src;

	if (dest == NULL && src == NULL)
		return (NULL);
	char_dest = (unsigned char *)dest;
	char_src = (const unsigned char *)src;
	while (n--)
		*char_dest++ = *char_src++;
	return (dest);
}

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int		operator;
	long	output;

	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	operator = 1;
	if (*str == '-')
	{
		operator *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	output = 0;
	while (ft_isdigit(*str))
	{
		output = output * 10 + (*str - '0');
		str++;
	}
	if (*str != '\0')
		return (-1);
	return ((int)(output * operator));
}
