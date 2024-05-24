/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:01:19 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/24 16:01:38 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
    sem_unlink("/die");
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
	open_sem(&shared_data);
	head = philos;
	shared_data.time = get_time();
	while (philos)
	{
		philos->pid = fork();
		if (philos->pid == -1)
		{
			printf ("problem at fork !\n");
			exit (1);
		}
		else if (philos->pid == 0)
			action((void *)philos);
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
	sem_close(shared_data.die_sem);
	sem_close(shared_data.print_sem);
	j = 0;
	t_philos *tmp;
	sem_close(shared_data.eats_sem);
	while (shared_data.head)
	{
		tmp = shared_data.head;
		sem_unlink(shared_data.head->name_sem);
		free(shared_data.head->name_sem);
		shared_data.head = shared_data.head->next;
		free(tmp);
	}
	free(shared_data.arr);
	return (0);
}