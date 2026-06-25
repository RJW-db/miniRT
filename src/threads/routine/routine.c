#include "scene.h"
#include "RTmlx.h"
#include "render.h"
#include "mathRT.h"
#include "threadsRT.h"
#include "setup_clean.h"

//	Static functions
static void	render_routine(t_thread *th);
static void	resynchronize_after_rendering(t_thread *th);

void	*thread_routine_init(t_thread *th)
{
	pthread_mutex_lock(th->rt->mtx + MTX_SYNC);
	pthread_mutex_unlock(th->rt->mtx + MTX_SYNC);
	render_routine(th);
	return (NULL);
}

static void	render_routine(t_thread *th)
{
	double	time;

	while (check_bool(th->rt->mtx + MTX_QUIT_ROUTINE, th->rt->quit_routine) == false)
	{
		time = mlx_get_time();
		if (th->rt->win->res_ratio == th->win->res_r_start)
		{
			thread_fast_render(th, th->rt->win);
			time = mlx_get_time() - time;
			th->win->delta_time = (float)time;
		}
		else if (thread_render(th, th->rt->win) == false)
		{
			time = mlx_get_time() - time;
			if (th->win->res_ratio == th->win->res_r_start - 1 &&
				check_bool(th->rt->mtx + MTX_SWITCH_BOOL, th->rt->win->auto_res))
				set_starting_res_ratio(th->rt, time);
		}
		print_performance_stats(th->rt, (float)(1.0 / time));
		resynchronize_after_rendering(th);
	}
	pthread_mutex_lock(th->rt->mtx + MTX_STOPPED_THREADS);
	++th->rt->stopped_threads;
	pthread_cond_signal(&th->rt->cond);
	pthread_mutex_unlock(th->rt->mtx + MTX_STOPPED_THREADS);
}

static void	resynchronize_after_rendering(t_thread *th)
{
	pthread_mutex_lock(th->rt->mtx + MTX_DONE_RENDERING);
	++th->rt->finished_rendering;
	pthread_mutex_unlock(th->rt->mtx + MTX_DONE_RENDERING);

	pthread_mutex_lock(th->rt->mtx + MTX_RESYNC);
	--th->rt->finished_rendering;
	pthread_cond_signal(&th->rt->cond);
	pthread_mutex_unlock(th->rt->mtx + MTX_RESYNC);

	pthread_mutex_lock(th->rt->mtx + MTX_SYNC);
	pthread_mutex_unlock(th->rt->mtx + MTX_SYNC);
}
