/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:20:33 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/13 11:29:06 by amejdoub         ###   ########.fr       */
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
	pthread_mutex_t		print;
	// pthread_mutex_t		meal;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		*forks;
	t_philos			*head;
	sem_t				*forks_sem;
	pid_t	pid;
	int *arr;
}						t_data;
typedef struct s_philos
{
	int					index;
	int					action_time;
	int					locked_forks;
	long long			time_to_eat;
	long long			time_to_sleep;
	long long			time_to_die;
	int					eating;
	int					left_fork;
	int					right_fork;
	long long			last_meal;
	pthread_t			thread;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		eating_mutex;
	t_data				*data;
	struct s_philos		*next;
}						t_philos;
int						kill_philos(t_philos *philos);
int						is_dead(t_philos *philos);
int	condition(t_philos *philos);
int	print(t_philos *philos, char *msg, int op);
#endif