/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amejdoub <amejdoub@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:12:13 by amejdoub          #+#    #+#             */
/*   Updated: 2024/05/26 14:49:27 by amejdoub         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char *argv[])
{
	if (argc > 4 && argc <= 6)
	{
		if (check_input(argv + 1))
		{
			printf("invalid args ! all args should be positive numbers\n");
			return (1);
		}
		simulation(argv + 1);
	}
	else
		return (printf("invalid number of args ! \n"));
	return (0);
}
