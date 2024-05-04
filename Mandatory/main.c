/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/04 15:25:44 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

t_philos	*get_by_index(t_philos *philos, int index)
{
	while (philos)
	{
		if (philos->index == index)
			return (philos);
		philos = philos->next;
	}
	return (NULL);
}

t_philos	*get_last_philo(t_philos *philos)
{
	while (philos->next)
	{
		philos = philos->next;
	}
	return (philos);
}

t_philos	*new_philo(char *data[], int index)
{
	t_philos	*new;

	new = malloc(sizeof(t_philos));
	if (!new)
		return (NULL);
	new->index = index + 1;
	new->time_to_die = ft_atoi(data[1]);
	new->time_to_eat = ft_atoi(data[2]);
	new->time_to_sleep = ft_atoi(data[3]);
    new->eating = 0;
	new->next = NULL;
	return (new);
}

void	add_philo_back(t_philos **philos, t_philos *new)
{
	if (!(*philos))
	{
		*philos = new;
	}
	else
	{
		get_last_philo(*philos)->next = new;
	}
}

void	fill_philos(char *data[], t_philos **philos)
{
	int	i;

	i = 0;
	while (i < ft_atoi(data[0]))
	{
		add_philo_back(philos, new_philo(data, i));
		i++;
	}
}

void *action(void *philos)
{
    // pthread_mutex_t mutex;
    t_philos *ph = (t_philos *)philos;

    if (ph && !ph->eating && ph->next && !ph->next->eating)
    {
		usleep(200);
        ph->eating = 1;
        ph->next->eating = 1;
		printf ("%d is eating\n", ph->index);
		ph->eating = 0;
		ph->next->eating = 0;
    }
    return (NULL);
}


void	simulation(char *data[])
{
	t_philos	*philos;

	philos = NULL;
	fill_philos(data, &philos);
    while (philos)
    {
        pthread_create(&philos->thread, NULL, action, (void *)philos);
        pthread_join(philos->thread, NULL);
        philos = philos->next;
    }

}

int	main(int argc, char *argv[])
{
	if (argc > 4 && argc <= 6)
	{
		// start(argv + 1);
		simulation(argv + 1);
	}
	else
		printf("invalid number of args ! \n");
	return (0);
}
