#include <stdio.h>
#include "libftx.h"
#include "miniRT.h"
#include "RTerror.h"
#define MTX_FAIL "pthread_mutex_destroy() failed on index "

void	destroy_mutexes(t_rt *rt, size_t amount)
{
	char	on_err[sizeof(MTX_FAIL) + 12];
	int		tmp_errno;
	ssize_t	i;

	i = (ssize_t)(amount - 1U);
	while (i > -1)
	{
		if (pthread_mutex_destroy(rt->mtx + i) != 0)
		{
			tmp_errno = errno;
			cpy_str0(on_err, MTX_FAIL);
			nbr_to_buff(on_err + sizeof(MTX_FAIL) - 1, (int64_t)i);
			errset(perr_msg("destroy_mutexes", (int16_t)tmp_errno, on_err));
		}
		--i;
	}
}

bool	destroy_conditions(t_rt *rt)
{
	if (pthread_cond_destroy(&rt->cond) != 0)
	{
		errset(perr("destroy_conditions", (int16_t)errno));
		return (false);
	}
	return (true);
}

void	destroy_threads(t_rt *rt)
{
	if (pthread_join(rt->thread.thread, NULL) != 0)
	{
		pthread_mutex_lock(rt->mtx + MTX_PRINT);
		errset(perr("destroy_threads", (int16_t)errno));
		pthread_mutex_unlock(rt->mtx + MTX_PRINT);
	}
}
