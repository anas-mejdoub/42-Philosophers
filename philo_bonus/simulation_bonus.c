/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:01:19 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/26 12:32:21 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	semaphore_unlink(void)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/death");
	sem_unlink("/begin");
	sem_unlink("/eat");
	sem_unlink("/die");
}

void	process_creation(t_philos *philos)
{
	int	i;

	i = 0;
	while (philos)
	{
		philos->pid = fork();
		if (philos->pid == -1)
		{
			printf("problem at fork !\n");
			exit(1);
		}
		else if (philos->pid == 0)
			action((void *)philos);
		else
		{
			philos->data->arr[i] = philos->pid;
			i++;
			philos = philos->next;
		}
	}
}

void	_end_(t_philos *philos)
{
	t_philos	**tmp;

	tmp = NULL;
	sem_wait(philos->data->death_sem);
	kill_process(philos->data);
	sem_close(philos->data->begin_sem);
	sem_close(philos->data->forks_sem);
	sem_close(philos->data->death_sem);
	sem_close(philos->data->die_sem);
	sem_close(philos->data->print_sem);
	sem_close(philos->data->eats_sem);
	while (philos->data->head)
	{
		sem_unlink(philos->data->head->name_sem);
		tmp = &philos->data->head;
		free(philos->data->head->name_sem);
		philos->data->head = philos->data->head->next;
		free(*tmp);
		*tmp = NULL;
	}
	free(philos->data->arr);
}

void	optional_param(t_philos *philos)
{
	if (philos->data->each_eat != -1)
	{
		pthread_create(&philos->data->eat_thread, NULL, (void *)eats_end,
			(void *)philos->data);
		pthread_detach(philos->data->eat_thread);
	}
}

int	simulation(char *data[])
{
	t_philos	*philos;
	t_data		shared_data;
	t_philos	*head;
	int			i;

	i = 0;
	semaphore_unlink();
	philos = NULL;
	shared_data.philos_number = 0;
	shared_data.died = 0;
	if (data[4])
		shared_data.each_eat = ft_atoi(data[4]);
	else
		shared_data.each_eat = -1;
	fill_philos(data, &philos, &shared_data);
	initial_data(philos, &shared_data);
	open_sem(&shared_data);
	head = philos;
	shared_data.time = get_time();
	process_creation(philos);
	optional_param(philos);
	_end_(philos);
	return (0);
}
