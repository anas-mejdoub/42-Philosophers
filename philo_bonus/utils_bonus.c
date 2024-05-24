/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:55:57 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/24 17:35:38 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
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

void	ft_sleep(long long time_to_sleep, t_philos *philos, int think)
{
	long long	time;

	if (think == 1)
	{
		sem_wait(philos->data->print_sem);
		printf("%lld %d is thinking\n", get_time() - philos->data->time,
			philos->index);
		sem_post(philos->data->print_sem);
	}
	time = get_time();
	while (1)
	{
		if (time + time_to_sleep <= get_time())
		{
			break ;
		}
		usleep(85);
	}
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = get_nblen(n) + 1;
	res = malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[len] = '\0';
	res[0] = '9';
	while (len-- != 0)
	{
		res[len] = (n % 10) + '0';
		n /= 10;
	}
	res[0] = '/';
	return (res);
}

long long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
