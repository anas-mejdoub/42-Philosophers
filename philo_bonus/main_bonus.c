/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/13 12:47:31 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_sleep(long long time_to_sleep, t_philos *philos, int think)
{
	long long	time;

	if (think == 1)
	{
		pthread_mutex_lock(&philos->data->print);
		printf("%lld %d is thinking\n", get_time() - philos->data->time, philos->index);
		pthread_mutex_unlock(&philos->data->print);
	}
	time = get_time();
	while (1)
	{
		if (time + time_to_sleep <= get_time())
		{
			break ;
		}
		else
		{
			pthread_mutex_lock(&philos->data->death_mutex);
			if (philos->data->died)
			{
				pthread_mutex_unlock(&philos->data->death_mutex);
				break ;
			}
			pthread_mutex_unlock(&philos->data->death_mutex);
		}
		usleep(50);
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
	int	mutex;

	mutex = 0;
	pthread_mutex_lock(&philos->data->print);
	if (op == 1)
	{
		if (condition(philos))
		{
			printf("%lld %d %s", get_time() - philos->data->time, philos->index,
				msg);
		}
		else
			return (pthread_mutex_unlock(&philos->data->print)
				, 0);
	}
	else if (op == 2)
	{
		pthread_mutex_lock(&philos->data->death_mutex);
		if (!philos->data->died)
		{
			pthread_mutex_unlock(&philos->data->death_mutex);
			pthread_mutex_lock(&philos->eating_mutex);
			philos->eating++;
			pthread_mutex_unlock(&philos->eating_mutex);
			printf("%lld %d %s%lld %d is eating\n", get_time()
				- philos->data->time, philos->index, msg, get_time()
				- philos->data->time, philos->index);
			mutex = 1;
		}
		else
			return (pthread_mutex_unlock(&philos->data->print),
				pthread_mutex_unlock(&philos->data->death_mutex), 0);
	}
	pthread_mutex_unlock(&philos->data->print);
	if (mutex)
	{
		ft_sleep(philos->time_to_eat, philos, 0);
	}
	return (1);
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
	new->action_time = 0;
	pthread_mutex_init(&new->meal_mutex, NULL);
	pthread_mutex_init(&new->eating_mutex, NULL);
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

int	is_dead(t_philos *philos)
{
	pthread_mutex_lock(&philos->meal_mutex);
	if (get_time() >= philos->last_meal + philos->time_to_die)
	{
		pthread_mutex_unlock(&philos->meal_mutex);
		pthread_mutex_lock(&philos->data->print);
		pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
		pthread_mutex_unlock(&philos->data->forks[philos->left_fork]);
		printf("%lld %d died\n", get_time() - philos->data->time,
			philos->index);
		pthread_mutex_lock(&philos->data->death_mutex);
		philos->data->died++;
		pthread_mutex_unlock(&philos->data->death_mutex);
		pthread_mutex_unlock(&philos->data->print);
		exit(1);
	}
	pthread_mutex_unlock(&philos->meal_mutex);
	return (0);
}
int	condition(t_philos *philos)
{
	pthread_mutex_lock(&philos->data->death_mutex);
	if (!philos->data->died)
	{
		pthread_mutex_unlock(&philos->data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philos->data->death_mutex);
	return (0);
}
void	*action(void *philos)
{
	t_philos	*ph;

	ph = (t_philos *)philos;
	if (ph->index % 2 == 0)
	{
		ph->action_time++;
		ft_sleep(10, philos, 1);
	}
	while (condition(philos))
	{
		if (!ph->action_time)
		{
			if (!print(ph, "is thinking\n", 1))
				break ;
		}
		ph->locked_forks = 0;
		sem_wait(ph->data->forks_sem);
		// pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
		ph->locked_forks++;
		if (!print(ph, "has taken a fork\n", 1))
		{
			pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
			break ;
		}
		// pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
		sem_wait(ph->data->forks_sem);
		ph->locked_forks++;
		// pthread_mutex_lock(&ph->meal_mutex);
		ph->last_meal = get_time();
		pthread_mutex_unlock(&ph->meal_mutex);
		if (!print(ph, "has taken a fork\n", 2))
		{
			break ;
		}
		sem_post(ph->data->forks_sem);
		sem_post(ph->data->forks_sem);
		// pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
		// pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
		if (!print(ph, "is sleeping\n", 1))
			break ;
		ft_sleep(ph->time_to_sleep, philos, 0);
		ph->action_time = 0;
	}
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
	data->forks_sem = sem_open("/philos_forks", O_CREAT, 0644, data->philos_number);
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
}

void	initial_data(t_philos *philos, t_data *shared_data)
{
	t_philos	*head;

	head = philos;
	shared_data->arr = malloc(shared_data->philos_number * sizeof(int));
	if (!shared_data->arr)
	{
		printf ("error at memory allocation\n");
		exit(1);
	}
	while (philos)
	{
		philos->data = shared_data;
		philos->data->head = head;
		philos = philos->next;
	}
}

void	free_destroy(t_philos *philos)
{
	int	i;

	i = 0;
	while (i < philos->data->philos_number)
	{
		pthread_mutex_destroy(&philos->data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(philos->data->forks);
	pthread_mutex_destroy(&philos->data->death_mutex);
	pthread_mutex_destroy(&philos->data->print);
	free(philos->data->forks);
	while (philos)
	{
		free(philos);
		philos = philos->next;
	}
}

void	unlock_all(t_philos *philos)
{
	int	i;

	i = 0;
	while (philos)
	{
		pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
		pthread_mutex_unlock(&philos->data->forks[philos->left_fork]);
		pthread_mutex_unlock(&philos->data->print);
		pthread_mutex_unlock(&philos->data->death_mutex);
		philos = philos->next;
		i++;
	}
}

int	kill_philos(t_philos *philos)
{
	t_philos	*temp;

	temp = philos;
	unlock_all(philos->data->head);
	while (philos)
	{
		pthread_join(philos->thread, NULL);
		philos = philos->next;
	}
	free_destroy(temp);
	return (1);
}

int	end_simulation(t_data *data)
{
	int	temp;

	pthread_mutex_lock(&data->head->eating_mutex);
	temp = data->head->eating;
	pthread_mutex_unlock(&data->head->eating_mutex);
	if (data->each_eat != -1)
	{
		while (data->head)
		{
			if (temp != data->each_eat || temp != data->head->eating)
				return (1);
			pthread_mutex_lock(&data->head->eating_mutex);
			temp = data->head->eating;
			pthread_mutex_unlock(&data->head->eating_mutex);
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
	t_philos	*head;
	t_philos	*head2;
	int i = 0;
	// t_philos	*head2;

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
		shared_data.pid = fork();
		if (shared_data.pid == -1)
		{
			printf ("problem at fork !\n");
			exit (1);
		}
		else if (shared_data.pid == 0)
			action((void *)philos);
		else
		{
			shared_data.arr[i] = shared_data.pid;
			i++;
			philos = philos->next;
		}
		// pthread_create(&philos->thread, NULL, action, (void *)philos);
	}
	i = 0;
	while (!shared_data.died)
	{
		i = 0;
		while (i < shared_data.philos_number)
		{
			// any process exited i gotta kill all
			// waitpid(shared_data.arr[i], )
		}
		// head2 = head;
		// while (head2)
		// {
		// 	if (is_dead(head2) || !end_simulation(head2->data))
		// 	{
		// 		pthread_mutex_lock(&shared_data.death_mutex);
		// 		shared_data.died++;
		// 		pthread_mutex_unlock(&shared_data.death_mutex);
		// 		break ;
		// 	}
		// 	head2 = head2->next;
		// }
	}
	return (kill_philos(head));
}

void	check_leaks(void)
{
	system("leaks philo");
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
