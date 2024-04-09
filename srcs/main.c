/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 01:51:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/09 21:05:35 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
		return (ft_error("incorrect number of params"), 1);
	if (set_table(argc, argv, &table))
		return (1);
	seat_diners(&table);
	dinner(&table);
	cleanup(&table);
	return (0);
}
