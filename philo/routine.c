/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:21:58 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/22 11:31:02 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*action(void *philos)
{
	t_philos	*ph;

	ph = (t_philos *)philos;
	if (ph->index % 2 == 0)
	{
		ph->action_time++;
		ft_sleep(10, philos, 1);
	}
	while (condition(philos))
	{
		if (!ph->action_time)
		{
			if (!print(ph, "is thinking\n", 1))
				break ;
		}
		ph->locked_forks = 0;
		pthread_mutex_lock(&ph->data->forks[ph->right_fork]);
		ph->locked_forks++;
		if (!print(ph, "has taken a fork\n", 1))
		{
			pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
			break ;
		}
		pthread_mutex_lock(&ph->data->forks[ph->left_fork]);
		ph->locked_forks++;
		pthread_mutex_lock(&ph->meal_mutex);
		ph->last_meal = get_time();
		pthread_mutex_unlock(&ph->meal_mutex);
		if (!print(ph, "has taken a fork\n", 2))
		{
			break ;
		}
		pthread_mutex_unlock(&ph->data->forks[ph->right_fork]);
		pthread_mutex_unlock(&ph->data->forks[ph->left_fork]);
		if (!print(ph, "is sleeping\n", 1))
			break ;
		ft_sleep(ph->time_to_sleep, philos, 0);
		ph->action_time = 0;
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

int	print(t_philos *philos, char *msg, int op)
{
	int	mutex;

	mutex = 0;
	pthread_mutex_lock(&philos->data->print);
	if (op == 1)
	{
		if (condition(philos))
		{
			printf("%lld %d %s", get_time() - philos->data->time, philos->index,
				msg);
		}
		else
			return (pthread_mutex_unlock(&philos->data->print), 0);
	}
	else if (op == 2)
	{
		pthread_mutex_lock(&philos->data->death_mutex);
		if (!philos->data->died)
		{
			pthread_mutex_unlock(&philos->data->death_mutex);
			pthread_mutex_lock(&philos->eating_mutex);
			philos->eating++;
			pthread_mutex_unlock(&philos->eating_mutex);
			printf("%lld %d %s%lld %d is eating\n", get_time()
				- philos->data->time, philos->index, msg, get_time()
				- philos->data->time, philos->index);
			mutex = 1;
		}
		else
			return (pthread_mutex_unlock(&philos->data->print),
				pthread_mutex_unlock(&philos->data->death_mutex), 0);
	}
	pthread_mutex_unlock(&philos->data->print);
	if (mutex)
	{
		ft_sleep(philos->time_to_eat, philos, 0);
	}
	return (1);
}
