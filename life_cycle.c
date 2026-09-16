/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:40:41 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:40:42 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

void	take_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->game->general_mutex);
	if (already_in(coder->game->heap, coder) == 0)
		insertHeap(coder->game->heap, coder);
	while (your_turn(coder, coder->game) == 0
		&& coder->game->burnout == false)
	{
		if (cooldown_check(coder) == 0)
		{
			waiter(coder);
		}
		else
			pthread_cond_wait (&coder->game->general_cond,
				&coder->game->general_mutex);
	}
	if (coder->game->burnout == true)
	{
		deleteKey(coder->game->heap, coder);
		pthread_mutex_unlock(&coder->game->general_mutex);
		return ;
	}
	dongle_state_change(coder);
	extract_root(coder->game->heap);
	pthread_mutex_unlock(&coder->game->general_mutex);
}

void	release_dongles(t_coder *coder)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	pthread_mutex_lock(&coder->game->general_mutex);
	coder->right_dongle->locked = false;
	coder->left_dongle->locked = false;
	coder->right_dongle->last_release = now;
	coder->left_dongle->last_release = now;
	if (coder->game->heap->scheduler == 1)
		buildHeap(coder->game->heap);
	pthread_cond_broadcast(&coder->game->general_cond);
	pthread_mutex_unlock(&coder->game->general_mutex);
}

void	compile(t_coder *coder)
{
	long	sleep_time;

	pthread_mutex_lock(&coder->game->general_mutex);
	sleep_time = coder->game->time_to_compile;
	gettimeofday(&coder->last_compile_start, NULL);
	state_info("comp", coder->id, coder->game->start_time);
	pthread_mutex_unlock(&coder->game->general_mutex);
	usleep(sleep_time * 1000);
	pthread_mutex_lock(&coder->game->general_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->game->general_mutex);
}

void	debug(t_coder *coder)
{
	long	sleep_time;

	pthread_mutex_lock(&coder->game->general_mutex);
	sleep_time = coder->game->time_to_debug;
	state_info("deb", coder->id, coder->game->start_time);
	pthread_mutex_unlock(&coder->game->general_mutex);
	usleep(sleep_time * 1000);
}

void	refactor(t_coder *coder)
{
	long	sleep_time;

	pthread_mutex_lock(&coder->game->general_mutex);
	sleep_time = coder->game->time_to_refactor;
	state_info("ref", coder->id, coder->game->start_time);
	pthread_mutex_unlock(&coder->game->general_mutex);
	usleep(sleep_time * 1000);
}