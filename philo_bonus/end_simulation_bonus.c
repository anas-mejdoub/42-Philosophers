/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_simulation_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:03:09 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/26 12:42:36 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_process(t_data *data)
{
	int	i;

	i = 0;
	usleep(100);
	while (i < data->philos_number)
	{
		kill(data->arr[i], SIGKILL);
		sem_close(get_by_index(data->head, i + 1)->meal_sem);
		i++;
	}
}

void	eats_end(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philos_number)
	{
		sem_wait(data->eats_sem);
		i++;
	}
	sem_post(data->death_sem);
	return ;
}
