/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:04:51 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/22 20:55:40 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_sleep(long long time_to_sleep, t_philos *philos, int think)
{
	long long	time;

	if (think == 1)
	{
		pthread_mutex_lock(&philos->data->print);
		printf("%lld %d is thinking\n", get_time() - philos->data->time,
			philos->index);
		pthread_mutex_unlock(&philos->data->print);
	}
	time = get_time();
	while (1)
	{
		if (time + time_to_sleep <= get_time())
			break ;
		else
		{
			pthread_mutex_lock(&philos->data->death_mutex);
			if (philos->data->died)
			{
				pthread_mutex_unlock(&philos->data->death_mutex);
				break ;
			}
			pthread_mutex_unlock(&philos->data->death_mutex);
		}
		usleep(50);
	}
}

int	ft_atoi(char *str)
{
	int	i;
	int	res;
	int	sign;

	i = 0;
	res = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * sign);
}

t_philos	*get_last_philo(t_philos *philos)
{
	while (philos->next)
	{
		philos = philos->next;
	}
	return (philos);
}
