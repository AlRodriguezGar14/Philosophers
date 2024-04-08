/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:42:01 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/09 00:32:25 by alberrod         ###   ########.fr       */
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

	if (!retrieve_times(&philo->philo_mutex, &philo->last_meal_time))
		update_value(&philo->philo_mutex, &philo->last_meal_time,
			&philo->table->start_time, sizeof(long));
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
//	long	curr_time;
//	int		rand_num;

	if (check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
		return ;
//	curr_time = get_time();
//	rand_num = ((int)curr_time % 3) + 3;
	write_status(THINK, philo);
//	precise_usleep(4, philo->table);
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
	while (!check_bool(&table->table_mutex, &table->threads_in_sync))
		;
	if (philo->id % 2)
		precise_usleep(42, table);
	philo->last_meal_time = philo->table->start_time;
	while (!check_bool(&philo->table->table_mutex, &philo->table->dinner_ended))
	{
		eat(philo);
		if (get_int(&philo->philo_mutex, &philo->counter_meals)
			== table->max_meals)
		{
			update_boolean(&philo->philo_mutex, &philo->has_eaten, true);
			increment_int(&table->table_mutex, &table->max_meals_achieved);
			if (get_int(&table->table_mutex, &table->max_meals_achieved)
				== table->number_of_philos)
				update_boolean(&table->table_mutex, &table->dinner_ended, true);
		}
		sleeping(philo);
		think(philo);
	}
	return (NULL);
}
