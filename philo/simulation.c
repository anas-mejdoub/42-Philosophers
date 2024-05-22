/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:24:05 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/22 20:52:24 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	threads_creation(t_philos *philos)
{
	while (philos)
	{
		if (pthread_create(&philos->thread, NULL, (void *)action,
				(void *)philos) != 0)
		{
			write(2, "error while creating threads\n", 30);
			kill_philos(philos->data->head);
			return (1);
		}
		philos = philos->next;
	}
	return (0);
}

int	death_watcher(t_data *shared_data)
{
	t_philos	*head2;

	while (!shared_data->died)
	{
		head2 = shared_data->head;
		while (head2)
		{
			if (is_dead(head2) || !end_simulation(head2->data))
			{
				pthread_mutex_lock(&shared_data->death_mutex);
				shared_data->died++;
				pthread_mutex_unlock(&shared_data->death_mutex);
				break ;
			}
			head2 = head2->next;
		}
	}
	return (0);
}

int	simulation(char *data[])
{
	t_philos	*philos;
	t_data		shared_data;
	t_philos	*head;

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
	get_last_philo(philos)->left_fork = 0;
	initial_data(philos, &shared_data);
	init_mutex(&shared_data);
	head = philos;
	if (threads_creation(philos))
		return (0);
	death_watcher(&shared_data);
	return (kill_philos(head));
}
