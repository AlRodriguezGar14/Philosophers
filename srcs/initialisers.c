/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:30:55 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/09 21:09:43 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	set_table(int argc, char **argv, t_table *table)
{
	if (argc != 6)
		table->max_meals = -1;
	else
	{
		table->max_meals = ft_atoi(argv[5]);
		if (table->max_meals < 0)
			return (ft_error("Not enough meals"), 1);
	}
	table->number_of_philos = ft_atoi(argv[1]);
	if (table->number_of_philos < 1)
		return (ft_error("No philos invited to the dinner."), 1);
	table->time_to_die = ft_atoi(argv[2]);
	if (table->time_to_die < 1)
		return (ft_error("Bruh, no time to die (God mode)."), 1);
	table->time_to_eat = ft_atoi(argv[3]);
	if (table->time_to_eat < 1)
		return (ft_error("No time to eat (famine mode)."), 1);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (table->time_to_sleep < 1)
		return (ft_error("No time to sleep (rave mode)."), 1);
	table->max_meals_achieved = 0;
	mutex_handler(&table->table_mutex, INIT);
	return (0);
}

void	assign_forks(t_philo *philo, int philo_seat)
{
	t_table	*table;

	table = philo->table;
	if (philo->id % 2)
	{
		philo->first_fork = &table->forks[philo_seat];
		philo->second_fork = &table->forks[(philo_seat + 1)
			% table->number_of_philos];
	}
	else
	{
		philo->first_fork = &table->forks[(philo_seat + 1)
			% table->number_of_philos];
		philo->second_fork = &table->forks[philo_seat];
	}
}

void	init_philo(t_table *table)
{
	int		idx;
	t_philo	*philo;

	idx = -1;
	while (++idx < table->number_of_philos)
	{
		philo = table->philos + idx;
		philo->id = idx + 1;
		philo->has_eaten = false;
		philo->counter_meals = 0;
		philo->last_meal_time = 0;
		philo->is_eating = false;
		philo->table = table;
		assign_forks(philo, idx);
		mutex_handler(&philo->philo_mutex, INIT);
	}
}

void	init_forks(t_table *table)
{
	int	idx;

	idx = -1;
	while (++idx < table->number_of_philos)
	{
		mutex_handler(&table->forks[idx].fork, INIT);
		table->forks[idx].fork_id = idx;
	}
}

void	seat_diners(t_table *table)
{
	table->dinner_ended = false;
	table->philos = malloc(sizeof(t_philo) * table->number_of_philos);
	if (!table->philos)
		ft_error("Oops, not everyone is on the table (malloc error)");
	table->forks = malloc(sizeof(t_fork) * table->number_of_philos);
	if (!table->forks)
		ft_error("Oops, missing forks (malloc error)");
	init_forks(table);
	init_philo(table);
}
