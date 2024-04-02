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
