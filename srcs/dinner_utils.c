/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 21:51:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/07 19:57:22 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:36:18 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/02 17:40:10 by alberrod         ###   ########.fr       */
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
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}

void	precise_usleep(int ms, t_table *table)
{
	struct timeval	start;
	struct timeval	current;
	int				elapsed;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec)
			* 1000
			+ (current.tv_usec - start.tv_usec)
			/ 1000;
		if (elapsed >= ms || check_bool(&table->table_mutex, &table->dinner_ended))
			break ;
		usleep(500);
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
