/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   states.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:40:56 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:40:57 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

void	state_info(char *state, int coder_id, struct timeval start_time)
{
	long	elapsed_time;
	char	*message;

	elapsed_time = get_elapsed_time(start_time);
	if (ft_strncmp(state, "taken") == 0)
		message = "has taken a dongle";
	else if (ft_strncmp(state, "comp") == 0)
		message = "is compiling";
	else if (ft_strncmp(state, "deb") == 0)
		message = "is debugging";
	else if (ft_strncmp(state, "ref") == 0)
		message = "is refactoring";
	else if (ft_strncmp(state, "bur") == 0)
		message = "burned out";
	printf("%ld %d %s\n", elapsed_time, coder_id, message);
}

int	your_turn(t_queue **head, t_coder *coder, t_data *game)
{
	if (cooldown_check(coder) == 0)
		return (0);
	if (coder->right_dongle->locked == true
		|| coder->left_dongle->locked == true)
		return (0);
	if (coder->right_dongle == coder->left_dongle)
		return (0);
	if (*head == NULL)
		return (1);
	if (game->heap->coder[0] != coder)
		return (0);
	return (1);
}

t_coder	first_expire(t_coder *a, t_coder *b)
{
	if (burnout_calculator(a) < burnout_calculator(b))
		return (a);
	return (b)
}

long	burnout_calculator(t_coder *coder)
{
	long	brn_time;

	brn_time = coder->game->time_to_burnout
		- get_elapsed_time(coder->last_compile_start);
	return (brn_time);
}

int		cooldown_check(t_coder *coder)
{
	struct 	timespec r_cooldown;
	struct 	timespec l_cooldown;
	struct 	timespec now;
	int 	res_left;
	int		res_right;

	if (coder->right_dongle->used == false
		&& coder->left_dongle->used == false)
			return (1);
	r_cooldown = cooldown_deadline(coder->game->dongle_cooldown, coder->right_dongle->last_release);
	l_cooldown = cooldown_deadline(coder->game->dongle_cooldown, coder->left_dongle->last_release);
	clock_gettime(CLOCK_REALTIME, &now);
	res_left = cooldown_check2(now, l_cooldown);
	res_right = cooldown_check2(now, r_cooldown);
	if (res_left == 1 && res_right == 1)
		return (1);
	return (0);
}