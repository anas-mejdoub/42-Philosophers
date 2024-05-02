/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/02 19:58:01 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi(char *str)
{
    int i = 0;
    int res = 0;
    int sign = 1;
    
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

t_philos *get_last_philo(t_philos *philos)
{
    while (philos->next)
    {
        philos = philos->next;
    }
    return (philos);
}

t_philos *new_philo(char *data[], int index)
{
    t_philos *new;
    
    new = malloc(sizeof(t_philos));
    if (!new)
        return (NULL);
    new->index = index + 1;
    new->time_to_die = ft_atoi(data[1]);
    new->time_to_eat = ft_atoi(data[2]);
    new->time_to_sleep = ft_atoi(data[3]);
    new->next = NULL;
    return (new);
}

void add_philo_back(t_philos **philos, t_philos *new)
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

void fill_philos(char *data[], t_philos **philos)
{
    int i;

    i = 0;
    while (i < ft_atoi(data[0]))
    {
        add_philo_back(philos, new_philo(data, i));
        i++;
    }
}

void simulation(char *data[])
{
    t_philos *philos;
    philos = NULL;
    fill_philos(data, &philos);
    while (philos)
    {
        printf("%d %d %d %d  \n", philos->index, philos->time_to_die, philos->time_to_eat, philos->time_to_sleep);
        philos = philos->next;
    }
}


int main(int argc, char *argv[])
{
    if (argc > 4 && argc <= 6)
    {
        // start(argv + 1);
        simulation(argv + 1);
    }
    else
        printf("invalid number of args ! \n");
    return 0;
}
