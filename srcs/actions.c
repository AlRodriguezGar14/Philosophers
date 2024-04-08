/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:42:01 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/09 01:00:19 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	long	tmp;

	while (philo->table->number_of_philos <= 1)
		if (check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
			return (mutex_handler(&philo->first_fork->fork, UNLOCK));
	mutex_handler(&philo->first_fork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo);
	mutex_handler(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo);
	update_boolean(&philo->philo_mutex, &philo->is_eating, true);
	write_status(EAT, philo);
	tmp = get_time();
	update_value(&philo->philo_mutex,
		&philo->last_meal_time, &tmp, sizeof(long));
	precise_usleep(philo->table->time_to_eat, philo->table);
	update_boolean(&philo->philo_mutex, &philo->is_eating, false);
	mutex_handler(&philo->first_fork->fork, UNLOCK);
	mutex_handler(&philo->second_fork->fork, UNLOCK);
	increment_int(&philo->philo_mutex, &philo->counter_meals);
}

void	think(t_philo *philo)
{
	if (check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
		return ;
	write_status(THINK, philo);
}

void	sleeping(t_philo *philo)
{
	if (check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
		return ;
	write_status(SLEEP, philo);
	precise_usleep(philo->table->time_to_sleep, philo->table);
}

static void	check_and_update_meals(t_philo *philo, t_table *table)
{
	if (get_int(&philo->philo_mutex, &philo->counter_meals) == table->max_meals)
	{
		update_boolean(&philo->philo_mutex, &philo->has_eaten, true);
		increment_int(&table->table_mutex, &table->max_meals_achieved);
		if (get_int(&table->table_mutex, &table->max_meals_achieved)
			== table->number_of_philos)
			update_boolean(&table->table_mutex, &table->dinner_ended, true);
	}
}

void	*start_dinner(void *data)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)data;
	table = philo->table;
	while (!check_bool(&table->table_mutex, &table->threads_in_sync))
		usleep(100);
	if (philo->id % 2)
		precise_usleep(42, table);
	philo->last_meal_time = philo->table->start_time;
	while (!check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
	{
		eat(philo);
		check_and_update_meals(philo, table);
		sleeping(philo);
		think(philo);
	}
	return (NULL);
}
