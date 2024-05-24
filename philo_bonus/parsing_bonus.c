/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:49:37 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/24 16:10:41 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_philos	*new_philo(char *data[], int index, t_data *shared_data)
{
	t_philos	*new;

	new = malloc(sizeof(t_philos));
	if (!new)
		return (NULL);
	new->index = index + 1;
	shared_data->philos_number++;
	new->time_to_die = ft_atoi(data[1]);
	new->time_to_eat = ft_atoi(data[2]);
	new->time_to_sleep = ft_atoi(data[3]);
	new->last_meal = get_time();
	new->action_time = 0;
	new->name_sem = ft_itoa(index + 1);
	new->meal_sem = sem_open(new->name_sem, O_CREAT, 0644, 1);
	if (new->meal_sem == SEM_FAILED)
	{
		printf("problem with sem\n");
		exit(99);
	}
	new->right_fork = index;
	new->left_fork = index + 1;
	new->eating = 0;
	new->next = NULL;
	return (new);
}

t_philos	*get_last_philo(t_philos *philos)
{
	while (philos->next)
	{
		philos = philos->next;
	}
	return (philos);
}

void	add_philo_back(t_philos **philos, t_philos *new)
{
	new->eating = 0;
	if (!(*philos))
	{
		*philos = new;
	}
	else
	{
		get_last_philo(*philos)->next = new;
	}
}

void	fill_philos(char *data[], t_philos **philos, t_data *shared_data)
{
	int	i;

	i = 0;
	while (i < ft_atoi(data[0]))
	{
		add_philo_back(philos, new_philo(data, i, shared_data));
		i++;
	}
}

int	check_input(char **data)
{
	int	i;

	i = 0;
	while (data[i])
	{
		if (ft_atoi(data[i]) <= 0)
			return (1);
		i++;
	}
	return (0);
}
