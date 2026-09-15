/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:40:48 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:40:49 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

void	queue_adding(t_queue **head, t_coder *coder)
{
	t_queue	*new;
	t_queue	*temp;

	temp = *head;
	if (already_in(head, coder) == 0)
		return ;
	new = malloc(sizeof(t_queue));
	new->coder = coder;
	new->next = NULL;
	if (temp == NULL)
	{
		new->prev = NULL;
		*head = new;
	}
	else
	{
		while (temp->next)
			temp = temp->next;
		temp->next = new;
		new->prev = temp;
	}
	return ;
}

int	already_in(t_queue **head, t_coder *coder)
{
	t_queue	*temp;

	temp = *head;
	while (temp)
	{
		if (temp->coder == coder)
			return (0);
		temp = temp->next;
	}
	return (1);
}

void	queue_leaving(t_queue **head, t_coder *coder)
{
	int		id;
	t_queue	*delete_me;
	t_queue	*temp;

	id = coder->id;
	temp = *head;
	while (temp && temp->coder->id != id)
	{
		temp = temp->next;
	}
	if (temp && temp->coder->id == id)
	{
		delete_me = temp;
		if (temp == *head)
			*head = temp->next;
		if (temp->next)
			temp->next->prev = temp->prev;
		if (temp->prev)
			temp->prev->next = temp->next;
		free(delete_me);
	}
	return ;
}

void	queue_clear(t_queue **head)
{
	t_queue	*temp;

	while (*head)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
}
