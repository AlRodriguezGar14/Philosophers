/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:36:18 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/09 01:00:25 by alberrod         ###   ########.fr       */
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

void	write_status(t_Philo_Status status, t_philo *philo)
{
	long	elapsed;

	mutex_handler(&philo->table->table_mutex, LOCK);
	elapsed = get_time() - philo->table->start_time;
	if ((status == TAKE_FIRST_FORK
			|| status == TAKE_SECOND_FORK)
		&& !philo->table->dinner_ended)
		printf(BLUE"%ld"RESET" %d has taken a fork\n", elapsed, philo->id);
	else if (status == EAT && !philo->table->dinner_ended)
		printf(CYAN"%ld"RESET" %d is eating\n", elapsed, philo->id);
	else if (status == SLEEP && !philo->table->dinner_ended)
		printf(GREEN"%ld"RESET" %d is sleeping\n", elapsed, philo->id);
	else if (status == THINK && !philo->table->dinner_ended)
		printf(YELLOW"%ld"RESET" %d is thinking\n", elapsed, philo->id);
	else if (status == DIE && !philo->table->dinner_ended)
		printf(RED"%ld"RESET" %d died\n", elapsed, philo->id);
	mutex_handler(&philo->table->table_mutex, UNLOCK);
}
