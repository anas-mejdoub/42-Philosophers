/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:20:33 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/17 18:15:27 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
#include <semaphore.h> 
#include <signal.h>

typedef struct s_philos	t_philos;
typedef struct s_data
{
	int					philos_number;
	int					each_eat;
	int					died;
	long long			time;

	t_philos			*head;
	sem_t				*forks_sem;
	sem_t				*print_sem;
	sem_t				*death_sem;
	sem_t				*pid_sem;
	sem_t				*begin_sem;
	pid_t	pid;
	int *arr;
}						t_data;
typedef struct s_philos
{
	pid_t pid;
	int					index;
	char				*name_sem;
	int					action_time;
	int					locked_forks;
	long long			time_to_eat;
	long long			time_to_sleep;
	long long			time_to_die;
	int					eating;
	int					left_fork;
	int					right_fork;
	long long			last_meal;
	sem_t 	*meal_sem;
	sem_t 	*thread_sem;
	pthread_t			thread;
	pthread_t			wtacher_thread;
	t_data				*data;
	struct s_philos		*next;
}						t_philos;
int						kill_philos(t_philos *philos);
int						is_dead(t_philos *philos);
int	condition(t_philos *philos);
int	print(t_philos *philos, char *msg, int op);
#endif