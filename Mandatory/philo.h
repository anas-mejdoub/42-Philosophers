/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:20:33 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/04 15:24:25 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct s_philos
{
    int index;
    int time_to_eat;
    int time_to_sleep;
    int time_to_die;
    int eating;
    pthread_t thread;
    // int number_times;
    struct s_philos *next;
} t_philos;

#endif