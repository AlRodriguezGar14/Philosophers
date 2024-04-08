/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_values.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 16:27:41 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/08 16:39:49 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_bool(pthread_mutex_t *mutex, const bool *val)
{
	bool	out;

	mutex_handler(mutex, LOCK);
	if (*val == false)
		out = false;
	else
		out = true;
	mutex_handler(mutex, UNLOCK);
	return (out);
}

long	retrieve_times(pthread_mutex_t *mutex, const long *val)
{
	long	out;

	mutex_handler(mutex, LOCK);
	out = *val;
	mutex_handler(mutex, UNLOCK);
	return (out);
}

int	get_int(pthread_mutex_t *mutex, const int *val)
{
	int	out;

	mutex_handler(mutex, LOCK);
	out = *val;
	mutex_handler(mutex, UNLOCK);
	return (out);
}
