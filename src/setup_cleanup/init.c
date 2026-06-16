#include <setup_clean.h>
#include <render.h>
#include <RTerror.h>
#include <mathRT.h>
#include <parsing.h>

#define NO_FILENAME "Give argument: ./miniRT <filename>\n"
#define SCREEN_ERROR "Screensize is too small\n"

void	init_main(t_rt *rt, t_scene *scn, t_scene *read_scn, t_window *win)
{
	float	screen_size_multiplier;

	screen_size_multiplier = ((float)(SCREEN_HEIGHT + SCREEN_WIDTH) * 0.000001F);
	ft_bzero(rt, sizeof(t_rt));
	ft_bzero(scn, sizeof(t_scene));
	ft_bzero(read_scn, sizeof(t_scene));
	ft_bzero(win, sizeof(t_window));
	errset((int64_t)&rt->errnum);
	rt->scene = scn;
	rt->read_scene = read_scn;
	rt->win = win;
	rt->win->res_ratio = 1;
	scn->render = true;
	rt->win->res_r_start = RES_RATIO_MAX;
	win->delta_time = 0.1F;
	win->target_time = 0.002F + screen_size_multiplier;
	win->auto_res = true;
	win->man_r_start = 10;
	rt->scene->shadow_grsize = 1;
	if (THREADS > 1)
		rt->scene->soft_shadows = false;
}

int16_t	setup_init_parsing(t_rt *rt, const int argc, const char *argv)
{
	if (argc != 2)
	{
		ft_putendl_fd(NO_FILENAME, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (SCREEN_WIDTH < 5 || SCREEN_HEIGHT < 5)
	{
		ft_putendl_fd(SCREEN_ERROR, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	else if (parse_scene_file(argv, rt->scene) != 0)
	{
		cleanup(rt);
		return (errset(ERTRN));
	}
	else if (windows_setup_mlx(rt) != 0)
	{
		cleanup(rt);
		return (perr("MLX", errset(ERTRN)));
	}
	rt->win->target_time += (((float)rt->scene->l.l_arr_size * 1.5f) \
		+ (float)rt->scene->o.o_arr_size) * 0.001f;
	rt->win->target_time = clamp(rt->win->target_time, 0.01F, 0.025F);
	return (EXIT_SUCCESS);
}

int32_t	multithreaded(t_rt *rt)
{
	if (init_read_scene(rt->scene, rt->read_scene))
	{
		perr("read_scene", errset(ERTRN));
		return (rt->errnum);
	}
	rt->mtx_init_check = true;
	if (initialize_mutexes(rt) == false)
		return (rt->errnum);
	if (initialize_conditions(rt) == rt->errnum)
		return (rt->errnum);
	rt->creation_check = true;
	if (launch_pthreads(rt) == false)
	{
		perr("Pthread", errset(ERTRN));
		return (cleanup(rt));
	}
	pthread_mutex_lock(rt->mtx + MTX_RESYNC);
	pthread_mutex_unlock(rt->mtx + MTX_SYNC);
	return (EXIT_SUCCESS);
}
