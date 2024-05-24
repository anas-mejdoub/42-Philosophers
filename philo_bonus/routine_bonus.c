/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:47:19 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/24 16:14:42 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	condition(t_philos *philos)
{
	sem_wait(philos->meal_sem);
	if (get_time() >= philos->last_meal + philos->time_to_die)
	{
		sem_wait(philos->data->die_sem);
		sem_post(philos->meal_sem);
		return (1);
	}
	sem_post(philos->meal_sem);
	return (0);
}

void	watcher(t_philos *philos)
{
	ft_sleep(50, philos, 0);
	while (1)
	{
		if (condition(philos))
		{
			printf("%lld %d died\n", get_time() - philos->data->time,
				philos->index);
			sem_post(philos->data->death_sem);
		}
		usleep(1000);
	}
}

void	*action(t_philos *philos)
{
	int	cycle;

	cycle = 1;
	pthread_create(&philos->thread, NULL, (void *)watcher, (void *)philos);
	pthread_detach(philos->thread);
	if (philos->index > philos->data->philos_number / 2
		&& philos->data->philos_number != 1)
	{
		print(philos, "is thinking\n", 1);
		cycle = 0;
		ft_sleep(4, philos, 0);
	}
	while (1)
	{
		if (cycle)
			print(philos, "is thinking\n", 1);
		cycle++;
		sem_wait(philos->data->forks_sem);
		print(philos, "has taken a fork\n", 1);
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
