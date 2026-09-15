/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:40:29 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:40:30 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

int	main(int argc, char **argv)
{
	t_data	game;
	int		assig_res;

	if (argc != 9)
	{
		printf("Inproper number of arguments\n");
		return (0);
	}
	assig_res = assigner_1(&game, argv);
	if (assig_res == 0)
		return (0);
	if (all_comps_finished(&game) == true)
		printf("All coders finished compiling. No burnout\n");
	destroyer(&game);
	return (0);
}

void	*life_cycle(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (permission2(coder) == 1)
	{
		take_dongles(coder);
		if (coder->game->burnout == true)
		{
			release_dongles(coder);
			return (NULL);
		}
		compile(coder);
		release_dongles(coder);
		if (coder->game->burnout == true)
			return (NULL);
		debug(coder);
		if (coder->game->burnout == true)
			return (NULL);
		refactor(coder);
	}
	return (NULL);
}

int	permission(t_coder *coder)
{
	if (coder->right_dongle->locked == true
		|| coder->left_dongle->locked == true)
		return (0);
	return (1);
}

int	permission2(t_coder *coder)
{
	pthread_mutex_lock(&coder->game->general_mutex);
	if (coder->compile_count < coder->game->num_of_compiles_required
		&& coder->game->burnout == false)
	{
		pthread_mutex_unlock(&coder->game->general_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coder->game->general_mutex);
	return (0);
}
