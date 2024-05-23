/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:21:58 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/23 11:04:37 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	thinking(t_philos *philos)
{
	if (!philos->action_time)
	{
		if (!print(philos, "is thinking\n", 1))
			return (1);
	}
	return (0);
}

int	taking_forks_eating(t_philos *philos)
{
	pthread_mutex_lock(&philos->data->forks[philos->right_fork]);
	philos->locked_forks++;
	if (!print(philos, "has taken a fork\n", 1))
		return (1);
	pthread_mutex_lock(&philos->data->forks[philos->left_fork]);
	philos->locked_forks++;
	pthread_mutex_lock(&philos->meal_mutex);
	philos->last_meal = get_time();
	pthread_mutex_unlock(&philos->meal_mutex);
	if (!print(philos, "has taken a fork\n", 2))
		return (1);
	pthread_mutex_unlock(&philos->data->forks[philos->right_fork]);
	pthread_mutex_unlock(&philos->data->forks[philos->left_fork]);
	return (0);
}

void	*action(t_philos *philos)
{
	if (philos->index % 2 == 0)
	{
		philos->action_time++;
		ft_sleep(10, philos, 1, get_time());
	}
	while (condition(philos))
	{
		if (thinking(philos))
			break ;
		philos->locked_forks = 0;
		if (taking_forks_eating(philos))
			break ;
		if (!print(philos, "is sleeping\n", 1))
			break ;
		ft_sleep(philos->time_to_sleep, philos, 0, get_time());
		philos->action_time = 0;
	}
	return (NULL);
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
