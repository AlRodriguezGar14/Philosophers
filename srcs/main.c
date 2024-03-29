/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 01:51:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/03/29 02:21:18 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


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

int main(int argc, char **argv)
{
    t_table table;

    if (argc < 5 || argc > 6)
        return (printf("incorrect number of params\n"), 1);
    set_table(argc, argv, &table);


    return (0);
}