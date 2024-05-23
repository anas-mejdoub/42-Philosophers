/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:21:02 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/23 11:05:49 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_take_eat(t_philos *philos, char *msg, int *mutex)
{
	pthread_mutex_lock(&philos->data->death_mutex);
	if (!philos->data->died)
	{
		pthread_mutex_unlock(&philos->data->death_mutex);
		pthread_mutex_lock(&philos->eating_mutex);
		philos->eating++;
		pthread_mutex_unlock(&philos->eating_mutex);
		printf("%lld %d %s%lld %d is eating\n", get_time() - philos->data->time,
			philos->index, msg, get_time() - philos->data->time, philos->index);
		*mutex = 1;
	}
	else
		return (pthread_mutex_unlock(&philos->data->print),
			pthread_mutex_unlock(&philos->data->death_mutex), 0);
	return (1);
}

int	print(t_philos *philos, char *msg, int op)
{
	int	mutex;

	mutex = 0;
	pthread_mutex_lock(&philos->data->print);
	if (op == 1)
	{
		if (condition(philos))
			printf("%lld %d %s", get_time() - philos->data->time, philos->index,
				msg);
		else
			return (pthread_mutex_unlock(&philos->data->print), 0);
	}
	else if (op == 2)
	{
		if (!print_take_eat(philos, msg, &mutex))
			return (0);
	}
	pthread_mutex_unlock(&philos->data->print);
	if (mutex)
		ft_sleep(philos->time_to_eat, philos, 0, get_time());
	return (1);
}
