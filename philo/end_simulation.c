/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:25:11 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/22 11:27:59 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	free_destroy(t_philos *philos)
{
	int			i;
	t_philos	*tmp;

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
		tmp = philos;
		philos = philos->next;
		free(tmp);
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
		return (1);
	}
	pthread_mutex_unlock(&philos->meal_mutex);
	return (0);
}
