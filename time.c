#include "codexion.h"

long	get_elapsed_time(struct timeval start)
{
	struct timeval	now;
	long			elapsed;

	gettimeofday(&now, NULL);
	elapsed = (
			(now.tv_sec * 1000000L + now.tv_usec)
			- (start.tv_sec * 1000000L + start.tv_usec)
			) / 1000;
	return (elapsed);
}

struct timespec	cooldown_deadline(long wait_time, struct timeval last_release)
{
	struct timespec deadline;

	deadline.tv_sec = last_release.tv_sec + wait_time / 1000;
	deadline.tv_nsec = last_release.tv_usec * 1000
					 + (wait_time % 1000) * 1000000;
	if (deadline.tv_nsec >= 1000000000)
	{
		deadline.tv_nsec -= 1000000000;
		deadline.tv_sec += 1;
	}
	return (deadline);
}

struct timespec	later_cooldown(struct timespec left_deadline, struct timespec right_deadline)
{
	if (left_deadline.tv_sec > right_deadline.tv_sec)
		return (left_deadline);
	if (left_deadline.tv_sec < right_deadline.tv_sec)
		return (right_deadline);
	if (left_deadline.tv_nsec >= right_deadline.tv_nsec)
		return (left_deadline);
	return (right_deadline);
}

int cooldown_check2(struct timespec now, struct timespec dongle)
{
	if (now.tv_sec > dongle.tv_sec)
		return (1);
	if (now.tv_sec == dongle.tv_sec)
		return (now.tv_nsec >= dongle.tv_nsec);
	return (0);
}

void waiter(t_coder *coder)
{
	struct timespec last;
	struct timespec left_deadline;
	struct timespec right_deadline;

	left_deadline = cooldown_deadline(coder->game->dongle_cooldown, coder->left_dongle->last_release);
	right_deadline = cooldown_deadline(coder->game->dongle_cooldown, coder->right_dongle->last_release);
	last = later_cooldown(left_deadline, right_deadline);

	pthread_cond_timedwait(&coder->game->general_cond, 
				&coder->game->general_mutex, 
				&last);
}