/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 21:33:06 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/06 00:21:45 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_values.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:28:01 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/02 17:31:49 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_value(pthread_mutex_t *mutex, void *var, void *val, size_t size)
{
	mutex_handler(mutex, LOCK);
	ft_memcpy(var, val, size);
	mutex_handler(mutex, UNLOCK);
}

void	update_boolean(pthread_mutex_t *mutex, bool *var, bool value)
{
	mutex_handler(mutex, LOCK);
	*var = value;
	mutex_handler(mutex, UNLOCK);
}

void	increment_int(pthread_mutex_t *mutex, int *var)
{
	mutex_handler(mutex, LOCK);
	(*var)++;
	mutex_handler(mutex, UNLOCK);
}

bool check_bool(pthread_mutex_t *mutex, bool *val)
{
	bool    out;

	mutex_handler(mutex, LOCK);
	if (*val == false)
		out = false;
	else
		out = true;
	mutex_handler(mutex, UNLOCK);
	return (out);
}

long retrieve_times(pthread_mutex_t *mutex, long *val)
{
	long out;

	mutex_handler(mutex, LOCK);
	out = *val;
	mutex_handler(mutex, UNLOCK);
	return (out);

}