/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 00:02:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/09 21:08:21 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_dinner(void *data)
{
	int				idx;
	t_table			*table;
	t_philo			*philo;
	pthread_mutex_t	*pmtx;

	table = (t_table *)data;
	precise_usleep(table->time_to_die, table);
	while (!check_bool(&table->table_mutex, &table->dinner_ended))
	{
		idx = -1;
		while (++idx < table->number_of_philos)
		{
			philo = &table->philos[idx];
			pmtx = &philo->philo_mutex;
			if (get_time() - retrieve_times(pmtx, &philo->last_meal_time)
				> table->time_to_die
				&& !check_bool(pmtx, &philo->is_eating))
			{
				write_status(DIE, philo);
				update_boolean(&table->table_mutex, &table->dinner_ended, true);
				break ;
			}
		}
	}
	return (NULL);
}

void	dinner(t_table *table)
{
	int		idx;
	long	tmp;

	table->threads_in_sync = false;
	table->dinner_ended = false;
	if (table->max_meals == 0)
		return ;
	idx = -1;
	while (++idx < table->number_of_philos)
		thread_handler(&table->philos[idx].thread_id,
			start_dinner,
			&table->philos[idx],
			CREATE);
	tmp = get_time();
	thread_handler(&table->monitor, monitor_dinner, table, CREATE);
	update_value(&table->table_mutex, &table->start_time, &tmp, sizeof(long));
	update_boolean(&table->table_mutex, &table->threads_in_sync, true);
	idx = -1;
	while (++idx < table->number_of_philos)
		thread_handler(&table->philos[idx].thread_id, NULL, NULL, JOIN);
	thread_handler(&table->monitor, NULL, NULL, JOIN);
}
