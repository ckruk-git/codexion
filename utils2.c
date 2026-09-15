/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:40:18 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:40:20 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

long	ft_atoi(const char *nptr)
{
	int		i;
	int		j;
	long	res;

	res = 0;
	i = 0;
	j = 1;
	while ((nptr[i] > 8 && nptr[i] < 14) || (nptr[i] == ' '))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i++] == '-')
			j = -1;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i++] - '0');
		if (res * j > INT_MAX || res * j < INT_MIN)
		{
			write(2, "Error\n", 6);
			exit(1);
		}
	}
	return (res * j);
}

long	ft_atol(const char *nptr)
{
	long	i[3];

	i[0] = 0;
	i[1] = 1;
	i[2] = 0;
	while ((nptr[i[0]] > 8 && nptr[i[0]] < 14) || (nptr[i[0]] == ' '))
		i[0]++;
	if (nptr[i[0]] == '-' || nptr[i[0]] == '+')
	{
		if (nptr[i[0]] == '-')
			i[1] = -1;
		i[0]++;
	}
	while (nptr[i[0]] >= '0' && nptr[i[0]] <= '9')
	{
		if (i[2] > (LONG_MAX - (nptr[i[0]] - '0')) / 10)
		{
			write(2, "Error\n", 6);
			exit(1);
		}
		i[2] = i[2] * 10 + (nptr[i[0]] - '0');
		i[0]++;
	}
	return (i[2] * i[1]);
}

char	*ft_tolower(char *a)
{
	int	i;

	i = 0;
	while (a[i])
	{
		if (a[i] > 64 && a[i] < 91)
		{
			a[i] = a[i] + 32;
		}
		i++;
	}
	return (a);
}

int	ft_strncmp(char *s1, char *s2)
{
	int	res;
	int	i;

	i = 0;
	while (s1[i])
	{
		if (s1[i] > s2[i])
		{
			res = s1[i] - s2[i];
			return (res);
		}
		else if (s1[i] < s2[i])
		{
			res = s2[i] - s1[i];
			res = res * (-1);
			return (res);
		}
		i++;
	}
	return (0);
}

void dongle_state_change (t_coder *coder)
{
	coder->right_dongle->locked = true;
	state_info("taken", coder->id, coder->game->start_time);
	coder->left_dongle->locked = true;
	state_info("taken", coder->id, coder->game->start_time);
	coder->right_dongle->used = true;
	coder->left_dongle->used = true;
}