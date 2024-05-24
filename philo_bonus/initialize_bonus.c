/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:54:20 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/24 16:10:09 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	open_sem(t_data *data)
{
	data->forks_sem = sem_open("/forks", O_CREAT, 0777, data->philos_number);
	data->print_sem = sem_open("/print", O_CREAT, 0777, 1);
	data->death_sem = sem_open("/death", O_CREAT, 0777, 0);
	data->begin_sem = sem_open("/begin", O_CREAT, 0777, 0);
	data->die_sem = sem_open("/die", O_CREAT, 0777, 1);
	data->eats_sem = sem_open("/eat", O_CREAT, 0777, 0);
	if (data->forks_sem == SEM_FAILED || data->print_sem == SEM_FAILED
		|| data->death_sem == SEM_FAILED)
	{
		write(2, "error with opening semaphores !", 32);
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
		printf("error at memory allocation\n");
		exit(1);
	}
	while (philos)
	{
		philos->data = shared_data;
		philos->data->head = head;
		philos = philos->next;
	}
}
