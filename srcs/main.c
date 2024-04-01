/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 01:51:53 by alberrod          #+#    #+#             */
/*   Updated: 2024/03/29 13:15:46by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    ft_error(char *str)
{
	printf(RED"%s\n"RESET, str);
	exit(EXIT_FAILURE);
}

void	precise_usleep(int ms, t_table *table)
{
    struct timeval start;
    struct timeval current;
    int elapsed;

    gettimeofday(&start, NULL);
    while (1)
    {
        gettimeofday(&current, NULL);
        elapsed = (current.tv_sec - start.tv_sec)
					* 1000
					+ (current.tv_usec - start.tv_usec)
					/ 1000;
        if (elapsed >= ms || table->dinner_ended)
            break;
        usleep(100); 
    }
}

typedef	enum	s_Mutex_Thread_Actions
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}   t_Mutex_Thread_Actions;

// TODO: Build a clean exit with different returns
void    handle_mutex_return(t_Mutex_Thread_Actions action, int status)
{
	if (!status)
		return ;
	else if (status == EINVAL && (action == LOCK || action == UNLOCK))
		ft_error("The value specified by mutex is invalid.");
	else if (status == EINVAL && action == INIT)
		ft_error("The value specified by attr is invalid.");
	else if (status == EDEADLK)
		ft_error("A deadlock would occur if the thread blocked waiting for mutex.");
	else if (status == EPERM)
		ft_error("The current thread does not hold a lock on mutex.");
	else if (status == ENOMEM)
		ft_error("The process cannot allocate enough memory to create another mutex.");
	else if (status == EBUSY)
		ft_error("Mutex is locked.");

}

void    mutex_handler(pthread_mutex_t *fork, t_Mutex_Thread_Actions action)
{
	if (action == LOCK)
		handle_mutex_return(action, pthread_mutex_lock(fork));
	else if (action == UNLOCK)
		handle_mutex_return(action, pthread_mutex_unlock(fork));
	else if (action == INIT)
		handle_mutex_return(action, pthread_mutex_init(fork, NULL));
	else if (action == DESTROY)
		handle_mutex_return(action, pthread_mutex_destroy(fork));
	else
		printf("TODO: No action to handle (mutex).\n");
}

void	handle_thread_error(t_Mutex_Thread_Actions action, int status)
{
	if (!status)
		return ;
	if (status == EPERM && action == CREATE)
		ft_error("The caller does not have appropriate permission to set the required scheduling parameters or scheduling policy.");
	else if (status == EAGAIN && action == CREATE)
		ft_error("The system lacked the necessary resources to create another thread, or the system-imposed limit on the total number of threads in a process [PTHREAD_THREADS_MAX] would be exceeded.");
	else if (status == EINVAL && action == CREATE)
		ft_error("The value specified by attr is invalid.");
	else if (status == EINVAL && (action == JOIN || action == DETACH))
		ft_error("The implementation has detected that the value specified by thread does not refer to a joinable thread.");
	else if (status == ESRCH && (action == JOIN || action == DETACH))
		ft_error("No thread could be found corresponding to that specified by the given thread ID, thread.");
	else if (status == EDEADLK && action == JOIN)
		ft_error("A deadlock was detected or the value of thread specifies the calling thread.");
}

void	thread_handler(pthread_t *thread, void *(*dinner)(void *), void *data, t_Mutex_Thread_Actions action)
{
	if (action == CREATE)
		handle_thread_error(pthread_create(thread, NULL, dinner, data), action);		
	else if (action == JOIN)
		handle_thread_error(pthread_join(*thread, NULL), action);
	else if (action == DETACH)
		handle_thread_error(pthread_detach(*thread), action);
	else
		printf("TODO: No action to handle (thread).\n");
	
}

void	update_value(pthread_mutex_t *mutex, void *var, void *value, size_t size)
{
	mutex_handler(mutex, LOCK);
	ft_memcpy(var, value, size);
	mutex_handler(mutex, UNLOCK);
}

void	update_boolean(pthread_mutex_t *mutex, bool *var, bool value)
{
	mutex_handler(mutex, LOCK);
	*var = value;
	mutex_handler(mutex, UNLOCK);
}

void	increment_int(pthread_mutex_t *mutex, int *var)
{
	mutex_handler(mutex, LOCK);
	(*var)++;
	mutex_handler(mutex, UNLOCK);

}

void	print_table_conditions(t_table *table)
{
	printf("Number of philos: %d\n", table->number_of_philos);
	printf("Time to die: %d\n", table->time_to_die);
	printf("Time to eat: %d\n", table->time_to_eat);
	printf("Time to sleep: %d\n", table->time_to_sleep);
	printf("Max meals: %d\n", table->max_meals);
}

void	set_table(int argc, char **argv, t_table *table)
{
	// TODO: Sanitize the input - check it is valid
	// TODO: No negative numbers as input allowed
	table->number_of_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->max_meals = ft_atoi(argv[5]);
	else
		table->max_meals = -1;
	table->max_meals_achieved = 0;
	print_table_conditions(table);
	if (table->time_to_die < 60
		|| table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		ft_error("There's no time to eat! (timestamp less than 60ms)");
	mutex_handler(&table->table_mutex, INIT);
}

void	assign_forks(t_philo *philo, int philo_seat)
{

	if (philo->id % 2)
	{
		philo->first_fork = &philo->table->forks[philo_seat];
		philo->second_fork = &philo->table->forks[(philo_seat + 1) % philo->table->number_of_philos];
	}
	else
	{
		philo->first_fork = &philo->table->forks[(philo_seat + 1) % philo->table->number_of_philos];
		philo->second_fork = &philo->table->forks[philo_seat];
	}
}

void	init_philo(t_table *table)
{
	int idx;
	t_philo *philo;

	idx = -1;
	printf("init philo\n");
	while (++idx < table->number_of_philos)
	{
		philo = table->philos + idx;
		philo->id = idx + 1;
		philo->has_eaten = false;
		philo->table = table;
		philo->has_eaten = false;
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
	init_forks(table); // TODO: Pending to destroy mutexes within forks when needed
	init_philo(table);
	

}
long	get_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}


typedef	enum	s_Philo_Status
{
	EAT,
	THINK,
	SLEEP,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIE,
}	t_Philo_Status;

void	write_status(t_Philo_Status status, t_philo *philo)
{
	long	elapsed;

	elapsed = get_time() - philo->table->start_time;

	mutex_handler(&philo->table->table_mutex, LOCK);
	
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK) && !philo->table->dinner_ended)
		printf(BLUE"%ld"RESET" %d has taken a fork\n", elapsed, philo->id);
	else if (status == EAT && !philo->table->dinner_ended )
		printf(CYAN"%ld"RESET" %d is eating\n", elapsed, philo->id);
	else if (status == SLEEP && !philo->table->dinner_ended )
		printf(GREEN"%ld"RESET" %d is sleeping\n", elapsed, philo->id);
	else if (status == THINK && !philo->table->dinner_ended )
		printf(YELLOW"%ld"RESET" %d is thinking\n", elapsed, philo->id);
	else if (status == DIE && !philo->table->dinner_ended)
		printf(RED"%ld"RESET" %d died\n", elapsed, philo->id);

	mutex_handler(&philo->table->table_mutex, UNLOCK);
}


void	eat(t_philo *philo)
{
    if (!philo->last_meal_time)
		update_value(&philo->philo_mutex,
				&philo->last_meal_time,
				&philo->table->start_time,
				sizeof(long));
    mutex_handler(&philo->first_fork->fork, LOCK);
    write_status(TAKE_FIRST_FORK, philo);
	if (philo->table->number_of_philos <= 1)
		precise_usleep(philo->table->time_to_die + 1, philo->table);
	else
	{
		mutex_handler(&philo->second_fork->fork, LOCK);
		write_status(TAKE_SECOND_FORK, philo);
		mutex_handler(&philo->philo_mutex, LOCK);
		philo->is_eating = true;
		philo->last_meal_time = get_time();
		write_status(EAT, philo);
		philo->counter_meals++;
		philo->is_eating = false;
		mutex_handler(&philo->philo_mutex, UNLOCK);
		precise_usleep(philo->table->time_to_eat, philo->table);

		mutex_handler(&philo->second_fork->fork, UNLOCK);
	}
	mutex_handler(&philo->first_fork->fork, UNLOCK);
}

void	think(t_philo *philo)
{
	write_status(THINK, philo);
	precise_usleep(1, philo->table);
}

void	sleeping(t_philo *philo)
{
	write_status(SLEEP, philo);
	precise_usleep(philo->table->time_to_sleep, philo->table);
}

void	*start_dinner(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (philo->table->threads_in_sync == false)
		;
	while (!philo->table->dinner_ended)
	{
		eat(philo);
		if (philo->counter_meals == philo->table->max_meals)
		{
			update_boolean(&philo->philo_mutex, &philo->has_eaten, true);
			increment_int(&philo->table->table_mutex,
			&philo->table->max_meals_achieved);
		}
		sleeping(philo);
		think(philo);
	}
	return (NULL);
}

void	*monitor_dinner(void *data)
{
    int		idx;
    long	elapsed;
    t_table	*table;
    
    table = (t_table *)data;
    while (table->threads_in_sync == false)
        ;
    precise_usleep(table->time_to_die, table);
    while (!table->dinner_ended)
    {
		if (table->max_meals_achieved == table->number_of_philos)
		{
			update_boolean(&table->table_mutex, &table->dinner_ended, true);
			break ;
		}
        idx = -1;
        while (++idx < table->number_of_philos)
        {
            if (!table->philos[idx].last_meal_time)
                continue ;
            elapsed = get_time() - table->philos[idx].last_meal_time;
            if (elapsed > table->time_to_die && !table->philos[idx].is_eating)
            {
                write_status(DIE, &table->philos[idx]);
				update_boolean(&table->table_mutex, &table->dinner_ended, true);
				break ;
            }
        }
    }
    return (NULL);
}

void	dinner(t_table *table)
{
	int	idx;

	table->threads_in_sync = false;
	table->dinner_ended = false;
	if (table->max_meals == 0)
		return ;
	idx = -1;
	while (++idx < table->number_of_philos)
		thread_handler(&table->philos[idx].thread_id,
					start_dinner,
					&table->philos[idx],
					CREATE);
	update_boolean(&table->table_mutex, &table->threads_in_sync, true);
	table->start_time = get_time();
	thread_handler(&table->monitor, monitor_dinner, table, CREATE);
	idx = -1;
	while (++idx < table->number_of_philos)
		thread_handler(&table->philos[idx].thread_id, NULL, NULL, JOIN);
	thread_handler(&table->monitor, NULL, NULL, JOIN);
	printf("Ladies and gentlement, the dinner ended\n");
}

void	cleanup(t_table *table)
{
	int	idx;

	idx = -1;
	while (++idx < table->number_of_philos)
	{
		mutex_handler(&table->philos[idx].philo_mutex, DESTROY);
		mutex_handler(&table->forks[idx].fork, DESTROY);
	}
	mutex_handler(&table->table_mutex, DESTROY);
	free(table->philos);
	free(table->forks);
}

int	main(int argc, char **argv)
{
	t_table table;

	if (argc < 5 || argc > 6)
		ft_error("incorrect number of params");
	set_table(argc, argv, &table);
	seat_diners(&table);
	dinner(&table);
	cleanup(&table);
	return (0);
}