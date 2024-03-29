/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 01:51:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/03/29 02:52:45 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    ft_error(char *str)
{
    printf(RED"%s\n", str);
    exit(EXIT_FAILURE);
}

void    print_table_conditions(t_table *table)
{
    printf("Number of philos: %d\n", table->number_of_philos);
    printf("Time to die: %d\n", table->time_to_die);
    printf("Time to eat: %d\n", table->time_to_eat);
    printf("Time to sleep: %d\n", table->time_to_sleep);
    printf("Max meals: %d\n", table->max_meals);
}

void    set_table(int argc, char **argv, t_table *table)
{
    table->number_of_philos = ft_atoi(argv[1]);
    table->time_to_die = ft_atoi(argv[2]);
    table->time_to_eat = ft_atoi(argv[3]);
    table->time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        table->max_meals = ft_atoi(argv[5]);
    else
        table->max_meals = -1;
    print_table_conditions(table);
}

void    seat_diners(t_table *table)
{
    t_philo *philo;

    philo = malloc(sizeof(philo) * table->number_of_philos);
    if (!philo)
        ft_error("Oops, someone couldn't got lost (malloc error)");
    table->philos = malloc(sizeof(philo) * table->number_of_philos);
    if (!table->philos)
        ft_error("Oops, not everyone is on the table (malloc error)");
    memset(philo, 0, sizeof(t_philo) * table->number_of_philos);
    // TODO: 
        // Init table
        // Init philo
        // Write a function to free both table and philos

}

int main(int argc, char **argv)
{
    t_table table;

    if (argc < 5 || argc > 6)
        ft_error(RED"incorrect number of params\n");
    set_table(argc, argv, &table);


    return (0);
}