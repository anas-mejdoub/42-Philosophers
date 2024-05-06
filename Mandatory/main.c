/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/06 19:51:52 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_sleep(long long time_to_sleep)
{
	long long	time;

	time = get_time();
	while (1)
	{
		if (time + time_to_sleep == get_time())
			break ;
	}
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

t_philos	*get_by_index(t_philos *philos, int index)
{
	while (philos)
	{
		if (philos->index == index)
			return (philos);
		philos = philos->next;
	}
	return (NULL);
}

t_philos	*get_last_philo(t_philos *philos)
{
	while (philos->next)
	{
		philos = philos->next;
	}
	return (philos);
}
void	print(t_philos *philos, char *msg, int op)
{
	pthread_mutex_lock(&philos->data->print);
	if (op == 1)
	{
		// philos->last_meal = get_time();
		printf("%lld %d %s", get_time() - philos->data->time, philos->index,
			msg);
	}
	else if (op == 2)
	{
		// philos->last_meal = get_time();
		printf("%lld %d %s%lld %d is eating\n", get_time() - philos->data->time,
			philos->index, msg, get_time() - philos->data->time, philos->index);
		ft_sleep(philos->time_to_eat);
	}
	pthread_mutex_unlock(&philos->data->print);
}
t_philos	*new_philo(char *data[], int index, t_data *shared_data)
{
	t_philos	*new;

	new = malloc(sizeof(t_philos));
	if (!new)
		return (NULL);
	new->index = index + 1;
	shared_data->philos_number++;
	new->time_to_die = ft_atoi(data[1]);
	new->time_to_eat = ft_atoi(data[2]);
	new->time_to_sleep = ft_atoi(data[3]);
	new->last_meal = get_time();
	// printf ("%lld time now\n", new->last_meal);
	new->right_fork = index;
	new->left_fork = index + 1;
	new->eating = 0;
	new->next = NULL;
	return (new);
}

void	add_philo_back(t_philos **philos, t_philos *new)
{
	if (!(*philos))
	{
		*philos = new;
	}
	else
	{
		get_last_philo(*philos)->next = new;
	}
}

void	fill_philos(char *data[], t_philos **philos, t_data *shared_data)
{
	int	i;

	i = 0;
	while (i < ft_atoi(data[0]))
	{
		add_philo_back(philos, new_philo(data, i, shared_data));
		i++;
	}
}
int is_dead(t_philos *philos)
{
	if (get_time() > philos->last_meal + philos->time_to_die)
		return 1;
	return 0;
}
void	*action(void *philos)
{
	t_philos	*ph;

	ph = (t_philos *)philos;
	if (ph->index % 2 == 0)
		usleep(15 * 1000);
	while (1)
	{
		print(ph, "is thinking\n", 1);
		pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
		print(ph, "has taken a fork\n", 1);
		pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
		ph->last_meal = get_time();
		print(ph, "has taken a fork\n", 2);
		pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
		pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
		print(ph, "is sleeping\n", 1);
		ft_sleep(ph->time_to_sleep);
		if (is_dead(philos))
		{
			print(ph, "is died \n", 1);
			kill_philos(ph->data->head);
			break;
		}
	}
	return (NULL);
}

void	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_number);
	if (!data->forks)
	{
		printf("err \n");
		exit(77);
	}
	while (i < data->philos_number)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->print, NULL);
}

void	initial_data(t_philos *philos, t_data *shared_data)
{
	t_philos *head = philos;
	while (philos)
	{
		philos->data = shared_data;
		philos->data->head = head;
		philos = philos->next;
	}
}
void kill_philos(t_philos *philos)
{
	while (philos)
	{
		pthread_join(philos->thread, NULL);
		philos = philos->next;
	}
}
void	simulation(char *data[])
{
	t_philos	*philos;
	t_data		shared_data;
	t_philos *head;

	philos = NULL;
	shared_data.philos_number = 0;
	shared_data.forks = NULL;
	shared_data.time = get_time();
	fill_philos(data, &philos, &shared_data);
	get_by_index(philos, ft_atoi(data[0]))->left_fork = 0;
	initial_data(philos, &shared_data);
	init_mutex(&shared_data);
	head = philos;
	if (!philos->data->forks)
	{
		printf("err with data \n");
		exit(90);
	}
	while (philos)
	{
		pthread_create(&philos->thread, NULL, action, (void *)philos);
		philos = philos->next;
	}
	kill_philos(head);
}

int	main(int argc, char *argv[])
{
	if (argc > 4 && argc <= 6)
	{
		simulation(argv + 1);
	}
	else
		printf("invalid number of args ! \n");
	return (0);
}
