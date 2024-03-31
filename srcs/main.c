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

void precise_usleep(int ms)
{
    struct timeval start, current;
    int elapsed;

    gettimeofday(&start, NULL);
    while (1)
    {
        gettimeofday(&current, NULL);
        elapsed = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec - start.tv_usec) / 1000;
        if (elapsed >= ms)
            break;
        usleep(100); // sleep for 100 microseconds to prevent high CPU usage
    }
}

typedef enum s_Mutex_Thread_Actions
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

void    print_table_conditions(t_table *table)
{
	printf("Number of philos: %d\n", table->number_of_philos);
	printf("Time to die: %d\n", table->time_to_die);
	printf("Time to eat: %d\n", table->time_to_eat);
	printf("Time to sleep: %d\n", table->time_to_sleep);
	printf("Max meals: %d\n", table->max_meals);
}

// void    set_table(int argc, char **argv, t_table *table)
// {
// 	// TODO: Sanitize the input - check it is valid
// 	// TODO: No negative numbers as input allowed
// 	table->number_of_philos = ft_atoi(argv[1]);
// 	table->time_to_die = ft_atoi(argv[2]) * 1000;
// 	table->time_to_eat = ft_atoi(argv[3]) * 1000;
// 	table->time_to_sleep = ft_atoi(argv[4]) * 1000;
// 	if (argc == 6)
// 		table->max_meals = ft_atoi(argv[5]);
// 	else
// 		table->max_meals = -1;
// 	print_table_conditions(table);
// 	if (table->time_to_die < 6e4
// 		|| table->time_to_eat < 6e4
// 		|| table->time_to_sleep < 6e4)
// 		ft_error("There's no time to eat! (timestamp less than 60ms)");
// 	mutex_handler(&table->table_mutex, INIT);
// }

void    set_table(int argc, char **argv, t_table *table)
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
	print_table_conditions(table);
	if (table->time_to_die < 60
		|| table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		ft_error("There's no time to eat! (timestamp less than 60ms)");
	mutex_handler(&table->table_mutex, INIT);
}

void  assign_forks(t_philo *philo, int philo_seat)
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

void    init_philo(t_table *table)
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
		philo->has_eaten = 0;
		assign_forks(philo, idx);
		printf("Philo %d owns the forks:\n\ta -> %d\n\tb-> %d\n", philo->id, philo->first_fork->fork_id, philo->second_fork->fork_id);
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

void    seat_diners(t_table *table)
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
long long	get_time()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}


typedef	enum s_Philo_Status
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
	long long	elapsed;

	elapsed = get_time() - philo->table->start_time;

	mutex_handler(&philo->table->table_mutex, LOCK);
	
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK) && !philo->table->dinner_ended)
		printf(BLUE"%lld"RESET" %d has taken a fork\n", elapsed, philo->id);
	else if (status == EAT && !philo->table->dinner_ended )
		printf(CYAN"%lld"RESET" %d is eating\n", elapsed, philo->id);
	else if (status == SLEEP && !philo->table->dinner_ended )
		printf(GREEN"%lld"RESET" %d is sleeping\n", elapsed, philo->id);
	else if (status == THINK && !philo->table->dinner_ended )
		printf(YELLOW"%lld"RESET" %d is thinking\n", elapsed, philo->id);
	else if (status == DIE)
		printf(RED"%lld"RESET" %d died\n", elapsed, philo->id);

	mutex_handler(&philo->table->table_mutex, UNLOCK);
}


void	eat(t_philo *philo)
{
    mutex_handler(&philo->philo_mutex, LOCK);
    if (!philo->last_meal_time)
        philo->last_meal_time = philo->table->start_time;
    mutex_handler(&philo->philo_mutex, UNLOCK);
    mutex_handler(&philo->first_fork->fork, LOCK);
    write_status(TAKE_FIRST_FORK, philo);
    mutex_handler(&philo->second_fork->fork, LOCK);
    write_status(TAKE_SECOND_FORK, philo);

    mutex_handler(&philo->philo_mutex, LOCK);
    if (philo->table->max_meals > 0 && philo->counter_meals >= philo->table->max_meals)
    {
        // TODO: End simulation.
        // exit (1);
    }
    philo->last_meal_time = get_time();
    write_status(EAT, philo);
    philo->counter_meals++;
    // TODO: Improve usleep
    // usleep(philo->table->time_to_eat * 1000);
	precise_usleep(philo->table->time_to_eat);
    mutex_handler(&philo->philo_mutex, UNLOCK);

    mutex_handler(&philo->first_fork->fork, UNLOCK);
    mutex_handler(&philo->second_fork->fork, UNLOCK);
}

void	think(t_philo *philo)
{
	write_status(THINK, philo);
	usleep(4200);
}

void	sleeping(t_philo *philo)
{
	write_status(SLEEP, philo);
	// TODO: Improve usleep
	// usleep(philo->table->time_to_sleep * 1000);
	precise_usleep(philo->table->time_to_sleep);
}

void	*start_dinner(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (philo->table->threads_in_sync == false)
		;
	while (!philo->table->dinner_ended)
	{
		if (philo->counter_meals == philo->table->max_meals)
			break ;
		eat(philo);
		sleeping(philo);
		think(philo);
	}
	return (NULL);
}

void	*monitor_dinner(void *data)
{
    int	idx;
    long long	elapsed;
    t_table *table;
    
    table = (t_table *)data;
	while (table->threads_in_sync == false)
		;
    while (1)
    {
        mutex_handler(&table->table_mutex, LOCK);
        if (table->dinner_ended)
        {
            mutex_handler(&table->table_mutex, UNLOCK);
            break;
        }
        mutex_handler(&table->table_mutex, UNLOCK);

        idx = -1;
        while (++idx < table->number_of_philos)
        {
            if (!table->philos[idx].last_meal_time)
                continue ;
			mutex_handler(&table->philos[idx].philo_mutex, LOCK);
            elapsed = get_time() - table->philos[idx].last_meal_time;
			mutex_handler(&table->philos[idx].philo_mutex, UNLOCK);
            if (elapsed > table->time_to_die && !table->philos[idx].is_eating)
            {
                write_status(DIE, &table->philos[idx]);
                exit (1);
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
	// else if (table->number_of_philos == 1)
	// 	return ;  // TODO: Function for just one filo
	idx = -1;
	while (++idx < table->number_of_philos)
		thread_handler(&table->philos[idx].thread_id, start_dinner, &table->philos[idx], CREATE);
	mutex_handler(&table->table_mutex, LOCK);
	table->threads_in_sync = true;
	mutex_handler(&table->table_mutex, UNLOCK);
	table->start_time = get_time();
	thread_handler(&table->monitor, monitor_dinner, table, CREATE);

	// wait the threads to end
	idx = -1;
	while (++idx < table->number_of_philos)
		thread_handler(&table->philos[idx].thread_id, NULL, NULL, JOIN);
}

int main(int argc, char **argv)
{
	t_table table;

	if (argc < 5 || argc > 6)
		ft_error("incorrect number of params");
	set_table(argc, argv, &table);
	seat_diners(&table);
	// TODO: Cleanup function for when the diner ends. (currently I have exit errors)
	// Pending to replace the exits with return values to como back here and free allocated memory (forks and philos)
	dinner(&table);

	return (0);
}