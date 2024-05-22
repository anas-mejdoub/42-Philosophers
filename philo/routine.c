/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:21:58 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/22 19:37:26 by amejdoub         ###   ########.fr       */
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
		ft_sleep(10, philos, 1);
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
		ft_sleep(philos->time_to_sleep, philos, 0);
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

// int	print(t_philos *philos, char *msg, int op)
// {
// 	int	mutex;

// 	mutex = 0;
// 	pthread_mutex_lock(&philos->data->print);
// 	if (op == 1)
// 	{
// 		if (condition(philos))
// 			printf("%lld %d %s", get_time() - philos->data->time, philos->index,
// 				msg);
// 		else
// 			return (pthread_mutex_unlock(&philos->data->print), 0);
// 	}
// 	else if (op == 2)
// 	{
// 		pthread_mutex_lock(&philos->data->death_mutex);
// 		if (!philos->data->died)
// 		{
// 			pthread_mutex_unlock(&philos->data->death_mutex);
// 			pthread_mutex_lock(&philos->eating_mutex);
// 			philos->eating++;
// 			pthread_mutex_unlock(&philos->eating_mutex);
// 			printf("%lld %d %s%lld %d is eating\n", get_time()
// 				- philos->data->time, philos->index, msg, get_time()
// 				- philos->data->time, philos->index);
// 			mutex = 1;
// 		}
// 		else
// 			return (pthread_mutex_unlock(&philos->data->print),
// 				pthread_mutex_unlock(&philos->data->death_mutex), 0);
// 	}
// 	pthread_mutex_unlock(&philos->data->print);
// 	if (mutex)
// 		ft_sleep(philos->time_to_eat, philos, 0);
// 	return (1);
// }
