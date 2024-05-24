/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:20:33 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/24 20:45:07 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
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
	t_philos			*head;
	sem_t				*forks_sem;
	sem_t				*print_sem;
	sem_t				*death_sem;
	sem_t				*pid_sem;
	sem_t				*begin_sem;
	sem_t				*eats_sem;
	sem_t				*die_sem;
	pthread_t			eat_thread;
	pid_t				pid;
	int					*arr;
}						t_data;
typedef struct s_philos
{
	pid_t				pid;
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
	sem_t				*meal_sem;
	sem_t				*thread_sem;
	pthread_t			thread;
	t_data				*data;
	struct s_philos		*next;
}						t_philos;
void					*action(t_philos *philos);
void					watcher(t_philos *philos);
int						condition(t_philos *philos);
void					add_philo_back(t_philos **philos, t_philos *new);
t_philos				*get_last_philo(t_philos *philos);
void					fill_philos(char *data[], t_philos **philos,
							t_data *shared_data);
int						check_input(char **data);
t_philos				*new_philo(char *data[], int index,
							t_data *shared_data);
int						kill_philos(t_philos *philos);
int						is_dead(t_philos *philos);
int						condition(t_philos *philos);
void					open_sem(t_data *data);
void					initial_data(t_philos *philos, t_data *shared_data);
int						ft_strlen(char *str);
int						print(t_philos *philos, char *msg, int op);
void					kill_process(t_data *data);
int						ft_atoi(char *str);
void					ft_sleep(long long time_to_sleep, t_philos *philos,
							int think);
char					*ft_itoa(int n);
t_philos				*get_by_index(t_philos *philos, int index);
void					eats_end(t_data *data);
int						get_nblen(int n);
int						simulation(char *data[]);
long long				get_time(void);
int						ft_is_digit(char *num);
#endif