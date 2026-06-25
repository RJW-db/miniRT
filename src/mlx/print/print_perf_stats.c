#include "scene.h"
#include "RTmlx.h"

void	switch_prt_perf_stats(t_rt *rt)
{
	pthread_mutex_lock(&rt->mtx[MTX_SWITCH_BOOL]);
	rt->win->prt_perf_stats = !rt->win->prt_perf_stats;
	pthread_mutex_unlock(&rt->mtx[MTX_SWITCH_BOOL]);
}
