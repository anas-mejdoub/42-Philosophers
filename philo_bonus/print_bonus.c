/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:05:10 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/24 17:39:52 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	print(t_philos *philos, char *msg, int op)
{
	int	mutex;

	mutex = 0;
	sem_wait(philos->data->print_sem);
	if (op == 1)
	{
		printf("%lld %d %s", get_time() - philos->data->time, philos->index,
			msg);
	}
	else if (op == 2)
	{
		sem_wait(philos->meal_sem);
		philos->last_meal = get_time();
		sem_post(philos->meal_sem);
		printf("%lld %d %s%lld %d is eating\n", get_time() - philos->data->time,
			philos->index, msg, get_time() - philos->data->time, philos->index);
		mutex = 1;
	}
	sem_post(philos->data->print_sem);
	if (mutex)
		ft_sleep(philos->time_to_eat, philos, 0);
	return (1);
}
