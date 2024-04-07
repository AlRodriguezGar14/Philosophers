/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 00:02:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/07 23:50:40 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:47:05 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/02 17:52:32 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_table_conditions(t_table *table)
{
	printf("Number of philos: %d\n", table->number_of_philos);
	printf("Time to die: %d\n", table->time_to_die);
	printf("Time to eat: %d\n", table->time_to_eat);
	printf("Time to sleep: %d\n", table->time_to_sleep);
	printf("Max meals: %d\n", table->max_meals);
}

void	*monitor_dinner(void *data)
{
	int		idx;
	long	elapsed;
	t_table	*table;

	table = (t_table *)data;
	precise_usleep(table->time_to_die, table);
	while (!check_bool(&table->table_mutex, &table->dinner_ended))
	{
		idx = -1;
		while (++idx < table->number_of_philos)
		{
			elapsed = get_time() - retrieve_times(&table->philos[idx].philo_mutex, &table->philos[idx].last_meal_time);
			if (elapsed > table->time_to_die && !check_bool(&table->philos[idx].philo_mutex, &table->philos[idx].is_eating))
			{
				write_status(DIE, &table->philos[idx]);
				update_boolean(&table->table_mutex, &table->dinner_ended, true);
				break ;
			}
		}
	}
	return (NULL);
}

void	dinner(t_table *table)
{
	int	idx;
	long tmp;

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
	update_value(&table->table_mutex, &table->start_time, &tmp, sizeof(long));
	update_boolean(&table->table_mutex, &table->threads_in_sync, true);
	thread_handler(&table->monitor, monitor_dinner, table, CREATE);
	idx = -1;
	while (++idx < table->number_of_philos)
		thread_handler(&table->philos[idx].thread_id, NULL, NULL, JOIN);
	thread_handler(&table->monitor, NULL, NULL, JOIN);
	printf("Ladies and gentlement, the dinner ended\n");
}
