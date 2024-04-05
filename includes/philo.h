/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 21:34:11 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/06 00:20:40 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 00:30:34 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/03 10:58:28 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <errno.h>

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

# define BLUE "\033[0;34m"
# define RESET "\033[0m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"

typedef struct s_fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}	t_fork;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	bool			has_eaten;
	int				counter_meals;
	long			last_meal_time;
	bool			is_eating;
	t_fork			*first_fork;
	t_fork			*second_fork;
	pthread_mutex_t	philo_mutex;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	int				number_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				max_meals_achieved;
	long			start_time;
	bool			dinner_ended;
	bool			threads_in_sync;
	int				nbr_of_threads_running;
	t_fork			*forks;
	t_philo			*philos;
	pthread_t		monitor;
	pthread_mutex_t	table_mutex;

}	t_table;

typedef enum s_Mutex_Thread_Actions
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_Mutex_Thread_Actions;

typedef enum s_Philo_Status
{
	EAT,
	THINK,
	SLEEP,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIE,
}	t_Philo_Status;

// utils.c
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_atoi(const char *str);

// dinner_utils.c
void	ft_error(char *str);
long	get_time(void);
void	precise_usleep(int ms, t_table *table);
void	cleanup(t_table *table);

// handlers.c
void	mutex_handler(pthread_mutex_t *mutex, t_Mutex_Thread_Actions action);
void	thread_handler(pthread_t *thread, void *(*dinner)(void *),
			void *data, t_Mutex_Thread_Actions action);

// update_values.c
void	update_value(pthread_mutex_t *mutex, void *var, void *val, size_t size);
void	update_boolean(pthread_mutex_t *mutex, bool *var, bool value);
void	increment_int(pthread_mutex_t *mutex, int *var);


bool check_bool(pthread_mutex_t *mutex, bool *val);
long retrieve_times(pthread_mutex_t *mutex, long *val);


// initialisers.c
void	set_table(int argc, char **argv, t_table *table);
void	assign_forks(t_philo *philo, int philo_seat);
void	init_philo(t_table *table);
void	init_forks(t_table *table);
void	seat_diners(t_table *table);

// actions.c
void	write_status(t_Philo_Status status, t_philo *philo);
void	eat(t_philo *philo);
void	think(t_philo *philo);
void	sleeping(t_philo *philo);
void	*start_dinner(void *data);

// dinner.c
void	print_table_conditions(t_table *table);
void	*monitor_dinner(void *data);
void	dinner(t_table *table);

#endif
