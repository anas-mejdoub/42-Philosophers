/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/18 20:54:06 by amejdoub         ###   ########.fr       */
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
		sem_wait(philos->meal_sem);
		philos->last_meal = get_time();
		sem_post(philos->meal_sem);
			printf("%lld %d %s%lld %d is eating\n", get_time()
				- philos->data->time, philos->index, msg, get_time()
				- philos->data->time, philos->index);
			mutex = 1;
	}
	sem_post(philos->data->print_sem);
	if (mutex)
		ft_sleep(philos->time_to_eat, philos, 0);
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
	new->name_sem = ft_itoa(index + 1);
	new->meal_sem = sem_open(new->name_sem, O_CREAT, 0644, 1);
	if (new->meal_sem == SEM_FAILED)
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
	new->eating = 0;
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
void time_for_philo(t_philos *philos)
{
	sem_wait(philos->data->begin_sem);
	
	sem_wait(philos->meal_sem);
	philos->last_meal = get_time();
	sem_post(philos->meal_sem);
	philos->data->time = get_time();
	sem_post(philos->data->begin_sem);
}

int condition (t_philos *philos)
{
	sem_wait(philos->meal_sem);
	if (get_time() >= philos->last_meal + philos->time_to_die)
	{
		sem_post(philos->meal_sem);
		sem_wait(philos->data->die_sem);
		sem_post(philos->data->die_sem);
		sem_post(philos->data->die_sem);
		return (1);
	}
	sem_post(philos->meal_sem);
	return (0);
}

void watcher(t_philos *philos)
{
	usleep(55 * 1000);
	while (1)
	{
		if (condition(philos))
		{
			sem_wait(philos->data->print_sem);
			printf("%lld %d died\n", get_time() - philos->data->time,
				philos->index);
			sem_post(philos->data->death_sem);
				printf ("{%d} done !\n", philos->index);
			while (1);
		}
		usleep(1000);
	}
}


void	*action(t_philos *philos)
{
	pthread_create(&philos->thread, NULL, (void *)watcher, (void *)philos);
	pthread_detach(philos->thread);
	time_for_philo(philos);
	if (philos->index > philos->data->philos_number / 2 && philos->data->philos_number != 1)
		usleep(4000);
	while (1)
	{
		print(philos, "is thinking\n", 1);
		sem_wait(philos->data->forks_sem);
		print(philos , "has taken a fork\n", 1);
		sem_wait(philos->data->forks_sem);
		print(philos, "has taken a fork\n", 2);
		philos->eating++;
		if (philos->eating == philos->data->each_eat)
		{
			sem_post(philos->data->eats_sem);
		}
		sem_post(philos->data->forks_sem);
		sem_post(philos->data->forks_sem);
		print(philos, "is sleeping\n", 1);
		ft_sleep(philos->time_to_sleep, philos, 0);
	}
	return (NULL);
}

void	init_mutex(t_data *data)
{
	data->forks_sem = sem_open("/forks", O_CREAT, 0777, data->philos_number);
	data->print_sem = sem_open("/print", O_CREAT, 0777, 1);
	data->death_sem = sem_open("/death", O_CREAT, 0777, 0);
	data->begin_sem = sem_open("/begin", O_CREAT, 0777, 0);
	data->die_sem = sem_open("/begin", O_CREAT, 0777, 1);
	data->eats_sem = sem_open("/eat", O_CREAT, 0777, 0);
	if (data->forks_sem == SEM_FAILED || data->print_sem == SEM_FAILED || data->death_sem == SEM_FAILED)
	{
		write (2, "error with opening semaphores !", 32);
		exit(1);
	}
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

void kill_process(t_data *data)
{
	int i = 0;
	while (i < data->philos_number)
	{
		sem_close(get_by_index(data->head, i + 1)->meal_sem);
		// printf ("%d\n", i);
		kill(data->arr[i], SIGKILL);
		i++;
	}
	// printf ("killed ?\n");
}
void eats_end(t_data *data)
{
	int i = 0;
	while (i < data->philos_number)
	{
		// printf("iiiiiiiiiii %d\n", i);
		sem_wait(data->eats_sem);
		i++;
	}
	sem_post(data->death_sem);
	while (1);
}

int	simulation(char *data[])
{
	t_philos	*philos;
	t_data		shared_data;
	t_philos	*head;
	
	int i = 0;
	sem_unlink("/forks");
    sem_unlink("/print");
    sem_unlink("/death");
    sem_unlink("/begin");
    sem_unlink("/eat");
	philos = NULL;
	shared_data.philos_number = 0;
	shared_data.died = 0;
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
		philos->pid = fork();
		if (philos->pid == -1)
		{
			printf ("problem at fork !\n");
			exit (1);
		}
		else if (philos->pid == 0)
		{
			action((void *)philos);
		}
		else
		{
			shared_data.arr[i] = philos->pid;
			i++;
			philos = philos->next;
		}
	}
	int j  = 0;
	while (j <= shared_data.philos_number)
	{
		sem_post(shared_data.begin_sem);
		j++;
	}
	if (shared_data.each_eat != -1)
	{
		pthread_create(&shared_data.eat_thread, NULL, (void *)eats_end, (void *)&shared_data);
		pthread_detach(shared_data.eat_thread);
	}
	sem_wait(shared_data.death_sem);
	kill_process(&shared_data);
	sem_close(shared_data.begin_sem);
	sem_close(shared_data.forks_sem);
	sem_close(shared_data.death_sem);
	sem_close(shared_data.print_sem);
	j = 0;
	while (j < shared_data.philos_number)
	{
		sem_post(shared_data.eats_sem);
		j++;
	}
	sem_close(shared_data.eats_sem);
	// printf("test1\n");
	while (shared_data.head)
	{
		sem_unlink(shared_data.head->name_sem);
		shared_data.head = shared_data.head->next; 
	}
	return (0);
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
