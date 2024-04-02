/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alberrod <alberrod@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:22:44 by alberrod          #+#    #+#             */
/*   Updated: 2024/04/02 17:26:48 by alberrod         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// TODO: Build a clean exit with different returns
static void	handle_mutex_return(t_Mutex_Thread_Actions action, int status)
{
	if (!status)
		return ;
	else if (status == EINVAL && (action == LOCK || action == UNLOCK))
		ft_error("The value specified by mutex is invalid.");
	else if (status == EINVAL && action == INIT)
		ft_error("The value specified by attr is invalid.");
	else if (status == EDEADLK)
		ft_error("A deadlock would occur if the thread blocked \
				waiting for mutex.");
	else if (status == EPERM)
		ft_error("The current thread does not hold a lock on mutex.");
	else if (status == ENOMEM)
		ft_error("The process cannot allocate enough memory \
				to create another mutex.");
	else if (status == EBUSY)
		ft_error("Mutex is locked.");
}

void	mutex_handler(pthread_mutex_t *mutex, t_Mutex_Thread_Actions action)
{
	if (action == LOCK)
		handle_mutex_return(action, pthread_mutex_lock(mutex));
	else if (action == UNLOCK)
		handle_mutex_return(action, pthread_mutex_unlock(mutex));
	else if (action == INIT)
		handle_mutex_return(action, pthread_mutex_init(mutex, NULL));
	else if (action == DESTROY)
		handle_mutex_return(action, pthread_mutex_destroy(mutex));
	else
		printf("TODO: No action to handle (mutex).\n");
}

static void	handle_thread_error(t_Mutex_Thread_Actions action, int status)
{
	if (!status)
		return ;
	if (status == EPERM && action == CREATE)
		ft_error("The caller does not have appropriate permission to set the \
				required scheduling parameters or scheduling policy.");
	else if (status == EAGAIN && action == CREATE)
		ft_error("The system lacked the necessary resources to create another \
				thread, or the system-imposed limit on the total number of \
				threads in a process [PTHREAD_THREADS_MAX] would be exceeded.");
	else if (status == EINVAL && action == CREATE)
		ft_error("The value specified by attr is invalid.");
	else if (status == EINVAL && (action == JOIN || action == DETACH))
		ft_error("The implementation has detected that the value specified by \
				thread does not refer to a joinable thread.");
	else if (status == ESRCH && (action == JOIN || action == DETACH))
		ft_error("No thread could be found corresponding to that specified by \
				the given thread ID, thread.");
	else if (status == EDEADLK && action == JOIN)
		ft_error("A deadlock was detected or the value of thread specifies the \
				calling thread.");
}

void	thread_handler(pthread_t *thread, void *(*dinner)(void *),
					void *data, t_Mutex_Thread_Actions action)
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
