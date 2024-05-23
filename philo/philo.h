/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:20:33 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/23 11:05:02 by amejdoub         ###   ########.fr       */
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
	int					each_eat;
	int					died;
	long long			time;
	pthread_mutex_t		print;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		*forks;
	t_philos			*head;
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
int						condition(t_philos *philos);
int						print(t_philos *philos, char *msg, int op);
long long				get_time(void);
void					ft_sleep(long long time_to_sleep, t_philos *philos,
							int think, long long time);
int						ft_atoi(char *str);
t_philos				*get_last_philo(t_philos *philos);
t_philos				*new_philo(char *data[], int index,
							t_data *shared_data);
void					add_philo_back(t_philos **philos, t_philos *new);
void					fill_philos(char *data[], t_philos **philos,
							t_data *shared_data);
int						check_input(char **data);
void					initial_data(t_philos *philos, t_data *shared_data);
void					init_mutex(t_data *data);
void					*action(t_philos *philos);
int						simulation(char *data[]);
int						end_simulation(t_data *data);
int						kill_philos(t_philos *philos);
void					unlock_all(t_philos *philos);
void					free_destroy(t_philos *philos);
int						is_dead(t_philos *philos);
int						print(t_philos *philos, char *msg, int op);
#endif