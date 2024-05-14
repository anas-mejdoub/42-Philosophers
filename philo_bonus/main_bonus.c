/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/14 17:47:34 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

static int	get_nblen(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = get_nblen(n) + 1;
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[len] = '\0';
	res[0] = '9';
	while (len-- != 0)
	{
		res[len] = (n % 10) + '0';
		n /= 10;
	}
	res[0] = '/';
	return (res);
}
void	ft_sleep(long long time_to_sleep, t_philos *philos, int think)
{
	long long	time;

	if (think == 1)
	{
		sem_wait(philos->data->print_sem);
		printf("%lld %d is thinking\n", get_time() - philos->data->time, philos->index);
		sem_post(philos->data->print_sem);
	}
	time = get_time();
	while (1)
	{
		if (time + time_to_sleep <= get_time())
		{
			break ;
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
	sem_wait(philos->data->print_sem);
	if (op == 1)
	{
			printf("%lld %d %s", get_time() - philos->data->time, philos->index,
				msg);
	}
	else if (op == 2)
	{
			philos->eating++;
			printf("%lld %d %s%lld %d is eating\n", get_time()
				- philos->data->time, philos->index, msg, get_time()
				- philos->data->time, philos->index);
			mutex = 1;
	}
	sem_post(philos->data->print_sem);
	if (mutex)
	{
		ft_sleep(philos->time_to_eat, philos, 0);
	}
	return (1);
}
int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

t_philos	*new_philo(char *data[], int index, t_data *shared_data)
{
	t_philos	*new;
	char *name;

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
	name = ft_itoa(index + 1);
	new->meal_sem = sem_open(name, O_CREAT, 0644, 1);
	if (!new->meal_sem)
	{
		printf ("problem with sem\n");
		exit (99);
	}
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
	if (get_time() >= philos->last_meal + philos->time_to_die)
	{
		sem_wait(philos->data->print_sem);
		printf("%lld %d died\n", get_time() - philos->data->time,
			philos->index);
		philos->data->died++;
		exit(1);
	}
	return (0);
}

int	condition(t_philos *philos)
{
	if (!philos->data->died)
	{
		return (1);
	}
	return (0);
}
void *watching(t_philos *philos)
{
	while (1)
	{
		sem_wait(philos->meal_sem);
		if (get_time() >= philos->last_meal + philos->time_to_die)
		{
			sem_post(philos->meal_sem);
			sem_wait(philos->data->print_sem);
			printf("%lld %d died\n", get_time() - philos->data->time,
				philos->index);
			philos->data->died++;
			pthread_join(philos->thread, NULL);
			sem_close(philos->meal_sem);
			exit(1);
		}
		sem_post(philos->meal_sem);
		usleep(50);
	}
	return (NULL);
}
void	*action(t_philos *philos)
{
	philos = (t_philos *)philos;
	pthread_create(&philos->thread, NULL, (void *)watching, (void *)philos);
	if (philos->index % 2 == 0)
	{
		philos->action_time++;
		ft_sleep(10, philos, 2);
	}
	while (1)
	{
		print(philos, "is thinking\n", 1);
		philos->locked_forks = 0;
		sem_wait(philos->data->forks_sem);
		philos->locked_forks++;
		print(philos , "has taken a fork\n", 1);
		sem_wait(philos->data->forks_sem);
		philos->locked_forks++;
		sem_wait(philos->meal_sem);
		philos->last_meal = get_time();
		sem_post(philos->meal_sem);
		if (!print(philos, "has taken a fork\n", 2))
		{
			break ;
		}
		sem_post(philos->data->forks_sem);
		sem_post(philos->data->forks_sem);
		if (!print(philos, "is sleeping\n", 1))
			break ;
		ft_sleep(philos->time_to_sleep, philos, 0);
		// is_dead(philos);
		philos->action_time = 0;
	}
	return (NULL);
}

void	init_mutex(t_data *data)
{
	data->forks_sem = sem_open("/forks", O_CREAT, 0777, data->philos_number);
	data->print_sem = sem_open("/print", O_CREAT, 0777, 1);
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

int	end_simulation(t_data *data)
{
	int	temp;

	temp = data->head->eating;
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
	t_philos	*head;
	
	int i = 0;


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
	}
	i = 0;
	// while (!shared_data.died)
	// {
	// 	i = 0;
		while (i < shared_data.philos_number)
		{
			wait(NULL);
			while (i < shared_data.philos_number)
			{
				// printf ("%d\n", i);
				kill(shared_data.arr[i], SIGKILL);
				i++;
			}
			break;
		}
	sem_close(shared_data.forks_sem);
	sem_close(shared_data.print_sem);
	return (1);
}

void	check_leaks(void)
{
	system("leaks philo");
}

int	main(int argc, char *argv[])
{


	// sem_unlink("/forks");
// sem_unlink("/print");
	if (argc > 4 && argc <= 6)
	{
		simulation(argv + 1);
	}
	else
		printf("invalid number of args ! \n");
	return (0);
	
}
