/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 01:51:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/02 17:53:20 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
		ft_error("incorrect number of params");
	set_table(argc, argv, &table);
	seat_diners(&table);
	dinner(&table);
	cleanup(&table);
	return (0);
}
