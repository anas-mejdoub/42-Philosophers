/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:24:05 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/22 11:36:03 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	simulation(char *data[])
{
	t_philos	*philos;
	t_data		shared_data;
	t_philos	*head;
	t_philos	*head2;

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
				pthread_mutex_lock(&shared_data.death_mutex);
				shared_data.died++;
				pthread_mutex_unlock(&shared_data.death_mutex);
				break ;
			}
			head2 = head2->next;
		}
	}
	return (kill_philos(head));
}
