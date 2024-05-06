/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:20:33 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/06 19:53:13 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philos	t_philos;
typedef struct s_data
{
	int					philos_number;
	long long			time;
	pthread_mutex_t		print;
	pthread_mutex_t		*forks;
	t_philos			*head;
}						t_data;
typedef struct s_philos
{
	int					index;
	int					time_to_eat;
	int					time_to_sleep;
	int					time_to_die;
	int					eating;
	int					left_fork;
	int					right_fork;
	long long			last_meal;
	pthread_t			thread;
	t_data				*data;
	struct s_philos		*next;
}						t_philos;
void					kill_philos(t_philos *philos);
#endif