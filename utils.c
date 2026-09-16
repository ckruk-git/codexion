/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:41:03 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:41:04 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

void	*monitor(void *arg)
{
	t_data			*game;
	int				i;

	game = (t_data *)arg;
	i = 0;
	while (true)
	{
		if (i >= game->num_of_coders)
			i = 0;
		pthread_mutex_lock(&game->general_mutex);
		if (i < game->num_of_coders
			&& game->coders[i].compile_count
			< game->num_of_compiles_required
			&& get_elapsed_time(game->coders[i].last_compile_start)
			> game->time_to_burnout)
		{
			state_info("bur", game->coders[i].id, game->start_time);
			pthread_mutex_unlock(&game->general_mutex);
			burnout(game);
			return (NULL);
		}
		pthread_mutex_unlock(&game->general_mutex);
		i++;
		if (all_comps_finished(game) == true)
			return (NULL);
		usleep(10);
	}
}

void	burnout(t_data *game)
{
	pthread_mutex_lock(&game->general_mutex);
	game->burnout = true;
	pthread_cond_broadcast(&game->general_cond);
	pthread_mutex_unlock(&game->general_mutex);
}

bool	all_comps_finished(t_data *game)
{
	int	i;
	int	complited;

	pthread_mutex_lock(&game->general_mutex);
	i = 0;
	complited = 0;
	while (i < game->num_of_coders)
	{
		if (game->coders[i].compile_count == game->num_of_compiles_required)
			complited++;
		i++;
	}
	pthread_mutex_unlock(&game->general_mutex);
	if (complited == game->num_of_coders)
		return (true);
	return (false);
}

void	destroyer(t_data *game)
{
	pthread_mutex_destroy(&game->general_mutex);
	pthread_cond_destroy(&game->general_cond);
	free(game->heap->coder);
    free(game->heap);
	free(game->coders);
	free(game->dongles);
}

