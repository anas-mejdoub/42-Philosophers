/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/05 14:27:31 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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
void print(t_philos *philos, char *msg)
{
	pthread_mutex_lock(&philos->data->print);
	printf("%d %s", philos->index, msg);
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

// void *action(void *philos)
// {
//     t_philos *ph = (t_philos *)philos;
// 	while (1)
// 	{
// 		pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
// 		pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
// 		printf("%d has taken a fork\n", ph->index);
// 		printf("%d has taken a fork\n", ph->index);
// 		printf("%d is eating\n", ph->index);
// 		usleep(ph->time_to_eat);
// 		pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
// 		pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
// 		printf("%d is sleeping\n", ph->index);
// 		usleep(ph->time_to_sleep);
// 		// break;
// 	}
//     return (NULL);
// }
void *action(void *philos)
{
    t_philos *ph = (t_philos *)philos;
	while (1)
	{
		if (ph->right_fork < ph->left_fork) {
			pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
			print(ph, "has taken a fork\n");
			pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
			print(ph, "has taken a fork\n");
			print(ph, "is eating\n");
			usleep(ph->time_to_eat);
		} else {
			pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
			print(ph, "has taken a fork\n");
			pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
			print(ph, "has taken a fork\n");
			print(ph, "is eating\n");
			usleep(ph->time_to_eat);
		}
		// usleep(ph->time_to_eat);
		pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
		pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
		print(ph, "is sleeping\n");
		usleep(ph->time_to_sleep);
		break;
	}
    return (NULL);
}
void init_mutex(t_data *data)
{
	int i = 0;

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

void initial_data(t_philos *philos , t_data *shared_data)
{
	while (philos)
	{
		philos->data = shared_data;
		philos = philos->next;
	}
}

void	simulation(char *data[])
{
	t_philos	*philos;
	t_data	shared_data;

	philos = NULL;
	t_philos *head;
	shared_data.philos_number = 0;
	shared_data.forks = NULL;
	fill_philos(data, &philos, &shared_data);
	get_by_index(philos, ft_atoi(data[0]))->left_fork = 0;
	initial_data(philos, &shared_data);
	init_mutex(&shared_data);
	head = philos;
	if (!philos->data->forks)
	{
		printf("err with data \n");
		exit (90);
	}
	
	while (philos)
    {
        pthread_create(&philos->thread, NULL, action, (void *)philos);
        // pthread_join(philos->thread, NULL);
        philos = philos->next;
    } 
    while (head)
    {
        // pthread_create(&philos->thread, NULL, action, (void *)philos);
        pthread_join(head->thread, NULL);
        head = head->next;
    }
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
