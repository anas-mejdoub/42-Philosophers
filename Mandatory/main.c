/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/08 19:54:35 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_sleep(long long time_to_sleep, t_philos *philos)
{
	long long	time;

	time = get_time();
	while (1)
	{
		if (time + time_to_sleep == get_time() || philos->data->died)
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
int	print(t_philos *philos, char *msg, int op)
{
	int mutex = 0;
	pthread_mutex_lock(&philos->data->print);
	if (op == 1)
	{
		if (!philos->data->died)
			printf("%lld %d %s", get_time() - philos->data->time, philos->index,
				msg);
		else
			return (pthread_mutex_unlock(&philos->data->print), 0);
			// return 0;
	}
	else if (op == 2)
	{
		if (!philos->data->died)
		{
			philos->eating++;
			printf("%lld %d %s%lld %d is eating\n", get_time() - philos->data->time,
				philos->index, msg, get_time() - philos->data->time, philos->index);
				mutex = 1;
		}
		else
			return (pthread_mutex_unlock(&philos->data->print), 0);
	}
	pthread_mutex_unlock(&philos->data->print);
	if (mutex)
		ft_sleep(philos->time_to_eat, philos);
	return 1;
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
	// t_philos *tmp = philos->data->head;
	if (get_time() >= philos->last_meal + philos->time_to_die)
	{
		// while (tmp)
		// {
		// 	if (tmp->index != philos->index)
		// 	{
		// 		pthread_mutex_unlock(&tmp->data->forks[tmp->right_fork]);
		// 		pthread_mutex_unlock(&tmp->data->forks[tmp->left_fork]);
		// 		pthread_mutex_unlock(&tmp->data->death_mutex);
		// 		// pthread_mutex_unlock(&tmp->data->print);
		// 	}
		// 	tmp = tmp->next;
		// }
		// 	tmp = tmp->next;
		// }
		pthread_mutex_lock(&philos->data->death_mutex);
		pthread_mutex_lock(&philos->data->print);
		// if (philos->locked_forks ==  1)
		// {
		// 	pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
		// }
		// else if (philos->locked_forks == 2)
		// {
			pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
			pthread_mutex_unlock(&philos->data->forks[philos->left_fork]);
		// }
		printf ("%lld %d died\n", get_time() - philos->data->time ,philos->index);
		philos->data->died++;
		// printf ("death %d \n", philos->data->died);
		pthread_mutex_unlock(&philos->data->print);
		pthread_mutex_unlock(&philos->data->death_mutex);
		// printf ("hhhhh\n");
		return 1;
	}
	return 0;
}

void	*action(void *philos)
{
	t_philos	*ph;

	ph = (t_philos *)philos;
	if (ph->index % 2 == 0)
		ft_sleep(10, philos);
	while (!ph->data->died)
	{
		if (!print(ph, "is thinking\n", 1))
			break;
		ph->locked_forks = 0;
		pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
		ph->locked_forks++;
		if (!print(ph, "has taken a fork\n", 1))
		{
			pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
			break;	
		}
		pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
		ph->locked_forks++;
		ph->last_meal = get_time();
		if (!print(ph, "has taken a fork\n", 2))
		{
			pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
			pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
			break;
		}
		pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
		// ph->locked_forks--;
		pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
		// ph->locked_forks--;
		if (!print(ph, "is sleeping\n", 1))
			break;
		ft_sleep(ph->time_to_sleep, philos);
	}
	// printf("--------------index %d\n", ph->index);
	return (NULL);
}

void	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philos_number);
	while (i < data->philos_number)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
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
void free_destroy(t_philos *philos)
{
	int i = 0;
	while (i < philos->data->philos_number)
	{
		pthread_mutex_destroy(&philos->data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(philos->data->forks);
	pthread_mutex_destroy(&philos->data->death_mutex);
	pthread_mutex_destroy(&philos->data->print);
	// free (philos->data->forks);
	// while (philos)
	// {
	// 	free (philos);
	// 	philos = philos->next;
	// }
	
}
void unlock_all(t_philos *philos)
{
	int i = 0;
	while (philos)
	{
		// int result = pthread_mutex_trylock(&philos->data->forks[philos->right_fork]);
		// if(result != 0)
			// printf(" 1 - valid\n");
		pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
		// result = pthread_mutex_trylock(&philos->data->forks[philos->left_fork]);
		// if(result != 0)
			// printf(" 2 %d - valid\n", result);
		pthread_mutex_unlock(&philos->data->forks[philos->left_fork]);
		// printf ("philo %d right %d left %d\n", philos->index, philos->right_fork, philos->left_fork);
		// result = pthread_mutex_trylock(&philos->data->print);
		// if(result != 0)
			// printf(" - valid\n");
		pthread_mutex_unlock(&philos->data->print);
		// result = pthread_mutex_trylock(&philos->data->death_mutex);
		// if(result != 0)
			// printf(" - valid\n");
		pthread_mutex_unlock(&philos->data->death_mutex);
		philos = philos->next;
		i++;
	}
	// printf("%d\n", i);
}
int kill_philos(t_philos *philos)
{
	// sleep(2);
	t_philos *temp = philos;
	unlock_all(philos->data->head);
	// exit (0);
	// int i  = 0;
	while (philos)
	{
		// while (i < philos->data->philos_number)
		// {
		// pthread_mutex_unlock(&philos->data->forks[i]);
		// i++;
		// }
		// pthread_mutex_unlock(&philos->data->forks[philos->left_fork]);
		// pthread_mutex_unlock(&philos->data->death_mutex);
		// pthread_mutex_unlock(&philos->data->print);
		// printf ("before \n");
		pthread_join(philos->thread, NULL);
		// printf ("join %d \n",pthread_join(philos->thread, NULL));
	// printf ("hhhhh3 %d next %d\n", philos->index, philos->next->index);
		philos = philos->next;
	}
	free_destroy(temp);
	return 1;
}

int end_simulation(t_data *data)
{
	int temp = data->head->eating;
	if (data->each_eat != -1)
	{
		while (data->head)
		{
			if (temp != data->each_eat || temp != data->head->eating)
				return (1);
			temp = data->head->eating;
			data->head = data->head->next;
		}
		if (!data->died)
			return (0);
	}
	return (1);
}

int	simulation(char *data[])
{
	t_philos	*philos;
	t_data		shared_data;
	t_philos *head;
	t_philos *head2;

	philos = NULL;
	shared_data.philos_number = 0;
	shared_data.forks = NULL;
	shared_data.died = 0;
	shared_data.time = get_time();
	if (data[4])
		shared_data.each_eat = ft_atoi(data[4]);
	else
		shared_data.each_eat = -1;
	fill_philos(data, &philos, &shared_data);
	get_by_index(philos, ft_atoi(data[0]))->left_fork = 0;
	initial_data(philos, &shared_data);
	init_mutex(&shared_data);
	head = philos;
	while (philos)
	{
		pthread_create(&philos->thread, NULL, action, (void *)philos);
		philos = philos->next;
	}
	while (!shared_data.died)
	{
		head2 = head;
		while (head2)
		{
			if (is_dead(head2) || !end_simulation(head2->data))
			{
				// printf ("hhh2\n");
				shared_data.died++;
				break;
			}
			head2 = head2->next;
		}
	}
			// printf ("hhhhh3\n");
	return (kill_philos(head));
}
void check_leaks()
{
	system("leaks philo");
}
int	main(int argc, char *argv[])
{
	// atexit (check_leaks);
	if (argc > 4 && argc <= 6)
	{
		simulation(argv + 1);
	}
	else
		printf("invalid number of args ! \n");
	return (0);
}
