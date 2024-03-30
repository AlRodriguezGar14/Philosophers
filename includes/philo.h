/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 00:30:34 by alberrod          #+#    #+#             */
/*   Updated: 2024/03/29 14:52:14 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "libft.h"
# include <stdbool.h>
# include <pthread.h> // mutex, thread
# include <sys/time.h> // gettimeofday
# include <limits.h> // INT_MAX
# include <errno.h>

# define BLUE "\033[0;34m"
# define RESET "\033[0m"
# define RED "\033[0;31m"
typedef struct s_fork
{
	pthread_mutex_t fork;
	int             fork_id;
}   t_fork;



typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	int				has_eaten;
	int				counter_meals; 
	int				ms_since_last_meal;
	t_fork			*first_fork;
	t_fork			*second_fork;
	pthread_mutex_t	philo_mutex; // for data races
	struct s_table	*table;
}					t_philo;


typedef	struct s_table
{
	int				number_of_philos;
	int				time_to_die; // time allowed from last meal to death
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals; // optional input. Max number of meals allowed <0 == no limit
	int				start_time;
	bool			dinner_ended;
	bool			threads_in_sync;
	int				nbr_of_threads_running;
	t_fork			*forks; // array of forks
	t_philo			*philos; // array of philos
	pthread_t		monitor;
	pthread_mutex_t	philos_table_dataraces_mutex;
	pthread_mutex_t	stdout_dataraces_mutex;

}	t_table;
# endif
