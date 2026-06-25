#include <stdio.h>
#include "libftx.h"
#include "threadsRT.h"
#include "render.h"
#include "RTerror.h"

bool	initialize_mutexes(t_rt *rt)
{
	size_t	i;

	i = 0;
	while (i < MTX_AMOUNT)
	{
		if (pthread_mutex_init(rt->mtx + i, NULL) != 0)
		{
			ft_putendl_fd("pthread_mutex_init: Failed\n", STDERR_FILENO);
			if (i > 0)
				destroy_mutexes(rt, i);
			rt->mtx_init_check = false;
			return (false);
		}
		++i;
	}
	return (true);
}

bool	initialize_conditions(t_rt *rt)
{
	if (pthread_cond_init(&rt->cond, NULL) != 0)
	{
		ft_putendl_fd("pthread_cond_init\n", STDERR_FILENO);
		return (false);
	}
	return (true);
}

bool	launch_pthreads(t_rt *rt)
{
	t_thread	*th;
	pthread_t	*thread;

	th = &rt->thread;
	thread = &rt->thread.thread;
	pthread_mutex_lock(rt->mtx + MTX_SYNC);
	th->rt = rt;
	th->scene = rt->read_scene;
	th->win = rt->win;
	if (pthread_create(thread, NULL, (t_cast)thread_routine_init, th) != 0)
	{
		pthread_mutex_lock(rt->mtx + MTX_PRINT);
		errset(perr("launh_pthreads", (int16_t)errno));
		pthread_mutex_unlock(rt->mtx + MTX_PRINT);
		rt->creation_check = false;
		pthread_mutex_unlock(rt->mtx + MTX_SYNC);
		return (false);
	}
	return (true);
}
