/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assigner.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:39:35 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:39:36 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

int	assigner_1(t_data *game, char **argv)
{
	if (ft_atoi(argv[1]) < 1)
	{
		write(1, "Min 1 coder required\n", 22);
		return (0);
	}
	game->num_of_coders = ft_atoi(argv[1]);
	game->time_to_burnout = ft_atol(argv[2]);
	game->time_to_compile = ft_atol(argv[3]);
	game->time_to_debug = ft_atol(argv[4]);
	game->time_to_refactor = ft_atol(argv[5]);
	game->num_of_compiles_required = ft_atoi(argv[6]);
	game->dongle_cooldown = ft_atoi(argv[7]);
	if (ft_strncmp(ft_tolower(argv[8]), "fifo") == 0)
		game->heap = createHeap(game->num_of_coders, 0);
	else if (ft_strncmp(ft_tolower(argv[8]), "edf") == 0)
		game->heap = createHeap(game->num_of_coders, 1);
	else
	{
		write(1, "Scheduler not recognised\n", 26);
		return (0);
	}
	game->burnout = false;
	game->head = NULL;
	coders_assigner(game);
	return (1);
}

void	coders_assigner(t_data *game)
{
	int	i;

	i = 0;
	gettimeofday(&game->start_time, NULL);
	game->coders = malloc(sizeof (t_coder) * game->num_of_coders);
	game->dongles = malloc(sizeof(t_dongle) * game->num_of_coders);
	while (i < game->num_of_coders)
	{
		game->coders[i].id = i + 1;
		game->dongles[i].id = i + 1;
		game->dongles[i].used = false;
		game->coders[i].compile_count = 0;
		game->coders[i].game = game;
		game->coders[i].last_compile_start = game->start_time;
		game->dongles[i].last_release = game->start_time;
		game->dongles[i].locked = false;
		game->coders[i].order = 0;
		i++;
	}
	dongles_assigner(game->coders, game->dongles, game->num_of_coders);
	threads_initializer(game);
}

void	threads_initializer(t_data *game)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	pthread_mutex_init(&game->general_mutex, NULL);
	pthread_cond_init(&game->general_cond, NULL);
	i = 0;
	while (i < game->num_of_coders)
	{
		pthread_create(&game->coders[i].thread, NULL,
			life_cycle, &game->coders[i]);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor, game);
	i = 0;
	while (i < game->num_of_coders)
		pthread_join(game->coders[i++].thread, NULL);
	pthread_join(monitor_thread, NULL);
	pthread_join(monitor_thread, NULL);
}

void	dongles_assigner(t_coder *coders,
		t_dongle *dongles_id, int num_of_coders)
{
	int	i;

	i = 1;
	while (i < num_of_coders)
	{
		coders[i].left_dongle = &dongles_id[i - 1];
		coders[i].right_dongle = &dongles_id[i];
		i++;
	}
	i--;
	coders[0].left_dongle = &dongles_id[i];
	coders[0].right_dongle = &dongles_id[0];
}
