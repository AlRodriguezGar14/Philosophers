/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 21:34:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/06 01:20:50 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:42:01 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/02 17:45:14 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	eat(t_philo *philo)
{
	long	tmp;

	if (!philo->last_meal_time)
		update_value(&philo->philo_mutex, &philo->last_meal_time,
			&philo->table->start_time, sizeof(long));
	mutex_handler(&philo->first_fork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo);
	while (philo->table->number_of_philos <= 1)
		if (check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
			return (mutex_handler(&philo->first_fork->fork, UNLOCK));
	mutex_handler(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo);
	update_boolean(&philo->philo_mutex, &philo->is_eating, true);
	tmp = get_time();
	update_value(&philo->philo_mutex,
		&philo->last_meal_time, &tmp, sizeof(long));
	write_status(EAT, philo);
	increment_int(&philo->philo_mutex, &philo->counter_meals);
	update_boolean(&philo->philo_mutex, &philo->is_eating, false);
	precise_usleep(philo->table->time_to_eat, philo->table);
	mutex_handler(&philo->second_fork->fork, UNLOCK);
	mutex_handler(&philo->first_fork->fork, UNLOCK);
}

void	think(t_philo *philo)
{
	if (check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
		return ;
	write_status(THINK, philo);
	precise_usleep(5, philo->table);
}

void	sleeping(t_philo *philo)
{
	if (check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
		return ;
	write_status(SLEEP, philo);
	precise_usleep(philo->table->time_to_sleep, philo->table);
}

void	*start_dinner(void *data)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)data;
	table = philo->table;
	//while (table->threads_in_sync == false)
	while (!check_bool(&table->table_mutex, &table->threads_in_sync))
		;

	while (!check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
	{
		eat(philo);
		if (philo->counter_meals == table->max_meals)
		{
			update_boolean(&philo->philo_mutex, &philo->has_eaten, true);
			increment_int(&table->table_mutex, &table->max_meals_achieved);
			if (table->max_meals_achieved == table->number_of_philos)
				update_boolean(&table->table_mutex, &table->dinner_ended, true);
		}
		sleeping(philo);
		think(philo);
	}
	return (NULL);
}