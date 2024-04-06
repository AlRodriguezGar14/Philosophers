/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:26:51 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/06 15:43:01 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:30:55 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/02 17:53:30 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_table(int argc, char **argv, t_table *table)
{
	if (argc != 6)
		table->max_meals = -1;
	else
	{
		table->max_meals = ft_atoi(argv[5]);
		if (table->max_meals < 0)
			ft_error("Not enough foods");
	}
	table->number_of_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->max_meals_achieved = 0;
	print_table_conditions(table);
	if (table->time_to_die < 0
		|| table->time_to_eat < 0
		|| table->time_to_sleep < 0)
		ft_error("There's no time to eat! (timestamp less than 0ms)");
	mutex_handler(&table->table_mutex, INIT);
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
	printf("init philo\n");
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
		printf("Philo %d owns the forks:\n\ta -> %d\n\tb-> %d\n",
			philo->id, philo->first_fork->fork_id, philo->second_fork->fork_id);
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
