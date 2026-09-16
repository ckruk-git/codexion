/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ckruk <ckruk@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/13 17:39:49 by ckruk             #+#    #+#             */
/*   Updated: 2026/08/13 17:39:50 by ckruk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <pthread.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_data	t_data;
typedef struct s_coder	t_coder;
typedef struct s_dongle	t_dongle;
typedef struct s_queue	t_queue;


typedef struct Heap
{
    t_coder **coder;
    int size;
    int capacity;
	int scheduler;
	long order;
} Heap;

typedef struct s_dongle
{
	int				id;
	bool			locked;
	bool			used;
	struct timeval 	last_release;
}	t_dongle;


typedef struct s_coder
{
	int				id;
	int				compile_count;
	long 			order;
	struct timeval	last_compile_start;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_data			*game;
}	t_coder;

typedef struct s_data
{
	int				num_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				num_of_compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	bool			burnout;
	struct timeval	start_time;
	pthread_mutex_t	general_mutex;
	pthread_cond_t	general_cond;
	t_coder			*coders;
	t_dongle		*dongles;
	struct Heap		*heap;
}	t_data;

//codexion.c
void	*life_cycle(void *arg);
int		permission(t_coder *coder);
int		permission2(t_coder *coder);

//assigner.c
int		assigner_1(t_data *game, char **argv);
void	coders_assigner(t_data *game);
void	threads_initializer(t_data *game);
void	dongles_assigner(t_coder *coders,
			t_dongle *dongles_id, int num_of_coders);

//utils2.c
long	ft_atoi(const char *nptr);
long	ft_atol(const char *nptr);
char	*ft_tolower(char	*a);
int		ft_strncmp(char *s1, char *s2);
void 	dongle_state_change (t_coder *coder);

//life_cycle.c
void	take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
void	compile(t_coder *coder);
void	debug(t_coder *coder);
void	refactor(t_coder *coder);

//states.c
void	state_info(char *state, int coder_id, struct timeval start_time);
int		your_turn(t_coder *coder, t_data *game);
t_coder	*first_expire(t_coder *a, t_coder *b);
long	burnout_calculator(t_coder *coder);
int		cooldown_check(t_coder *coder);


//utils.c
void	*monitor(void *arg);
void	burnout(t_data *game);
bool	all_comps_finished(t_data *game);
void	destroyer(t_data *game);
struct	timespec	cooldown_deadline(long wait_time, struct timeval last_release);

//time.c
long	get_elapsed_time(struct timeval start);
struct 	timespec	cooldown_deadline(long wait_time, struct timeval last_release);
struct 	timespec	later_cooldown(struct timespec left_deadline, struct timespec right_dealine);
int 	cooldown_check2(struct timespec now, struct timespec dongle);
void 	waiter(t_coder *coder);

//heap.c
Heap *createHeap(int capacity, int scheduler);
void swap(t_coder **a, t_coder **b);
void heapify(Heap *heap, int i);
void buildHeap(Heap *heap);
void insertHeap(Heap *heap, t_coder *coder);

//heap2.c
int extract_root(Heap *heap);
void deleteKey(Heap *heap, t_coder *coder);
int already_in(Heap *heap, t_coder *coder);
int higher_priority(t_coder *a, t_coder *b);

#endif