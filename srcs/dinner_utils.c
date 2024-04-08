/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:36:18 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/08 21:20:53 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_error(char *str)
{
	printf(RED"%s\n"RESET, str);
	exit(EXIT_FAILURE);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + (time.tv_usec / 1000));
}

void	precise_usleep(int ms, t_table *table)
{
	long	start;

	start = get_time();
	while (42)
	{
		if (get_time() - start >= (long)ms
			|| check_bool(&table->table_mutex, &table->dinner_ended))
			break ;
		usleep(100);
	}
}

void	cleanup(t_table *table)
{
	int	idx;

	idx = -1;
	while (++idx < table->number_of_philos)
	{
		mutex_handler(&table->philos[idx].philo_mutex, DESTROY);
		mutex_handler(&table->forks[idx].fork, DESTROY);
	}
	mutex_handler(&table->table_mutex, DESTROY);
	free(table->philos);
	free(table->forks);
}
