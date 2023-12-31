/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmanuel- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 14:03:47 by dmanuel-          #+#    #+#             */
/*   Updated: 2023/08/31 11:47:41 by dmanuel-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/philo.h"

int	one_philo(t_data *data)
{
	data->start_time = get_time();
	if (pthread_create(&data->tid[0], NULL, &routine, &data->philos[0]))
		return (error(TH_ERR, data));
	pthread_detach(data->tid[0]);
	pthread_mutex_lock(&data->lock);
	while (data->dead == 0)
	{
		pthread_mutex_unlock(&data->lock);
		usleep(1000);
		pthread_mutex_lock(&data->lock);
	}
	pthread_mutex_unlock(&data->lock);
	ft_exit(data);
	return (0);
}

void	ft_exit(t_data *data)
{
	int	i;

	if (!data)
		return ;
	i = -1;
	while (++i < data->philos_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos[i].lock);
	}
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	if (data->tid)
		free(data->tid);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}

int	error(char *str, t_data *data)
{
	printf("%s\n", str);
	if (data)
		ft_exit(data);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (1);
	if (checker(argv))
		return (1);
	if (init(&data, argv, argc))
		return (1);
	if (data.philos_num == 1)
	{
		data.solo = 0;
		return (one_philo(&data));
	}
	if (thread_init(&data))
		return (1);
	ft_exit(&data);
	return (0);
}
