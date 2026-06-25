#include "RTmlx.h"
#include "render.h"
#include "mathRT.h"

//	Static Functions
static void	closing_cleanup_threads(t_rt *rt);
static void	scene_manipulate_keys(mlx_key_data_t keydata, t_rt *rt);

void	init_hooks(t_rt *rt)
{
	mlx_t	*mlx;

	mlx = rt->win->mlx;
	if (THREADS > 1)
	{
		mlx_loop_hook(mlx, (mlx_closefunc)loop_hook_threaded, rt);
		mlx_close_hook(mlx, (mlx_closefunc)closing_cleanup_threads, rt);
	}
	else
		mlx_loop_hook(mlx, (mlx_closefunc)loop_hook, rt);
	mlx_key_hook(mlx, (mlx_keyfunc)my_keyhook, rt);
	mlx_scroll_hook(mlx, (mlx_cursorfunc)scroll_fov_hook, rt->scene);
	mlx_mouse_hook(mlx, (mlx_mousefunc)mouse_hook, rt);
	reset_filename(rt->win);
}

void	my_keyhook(mlx_key_data_t keydata, t_rt *rt)
{
	if (keydata.action != MLX_PRESS)
		return ;
	if (keydata.key == MLX_KEY_ENTER || rt->win->file_creation == true)
		set_filename(keydata.key, rt->win, rt->scene);
	else if (keydata.key == MLX_KEY_P)
		print_obj_info(rt->scene);
	else if (keydata.key == MLX_KEY_1)
		switch_scaling_mode(rt);
	else if (keydata.key == MLX_KEY_2 || keydata.key == MLX_KEY_3)
		change_scaling_start(rt, keydata);
	else if (keydata.key == MLX_KEY_K && THREADS > 1)
		switch_prt_perf_stats(rt);
	else if (keydata.key == MLX_KEY_ESCAPE)
	{
		if (THREADS > 1)
			closing_cleanup_threads(rt);
		mlx_close_window(rt->win->mlx);
	}
	else
		scene_manipulate_keys(keydata, rt);
}

void	brightness_keys(mlx_key_data_t keydata, t_rt *rt)
{
	t_scene	*sc;
	float	step;
	float	val;

	if (keydata.key != MLX_KEY_EQUAL && keydata.key != MLX_KEY_MINUS)
		return ;
	sc = rt->scene;
	step = 0.05F;
	if (keydata.key == MLX_KEY_MINUS)
		step = -step;
	if (sc->selected_obj != NULL && sc->selected_obj->type == LIGHT)
	{
		val = sc->selected_obj->u.l.brightness + step;
		sc->selected_obj->u.l.brightness = clamp(val, 0.0F, 1.0F);
	}
	else
	{
		val = sc->ambient.u.a.ratio + step;
		sc->ambient.u.a.ratio = clamp(val, 0.0F, 1.0F);
	}
	sc->render = true;
}

static void	scene_manipulate_keys(mlx_key_data_t keydata, t_rt *rt)
{
	uint16_t	idx;
	t_objs		*light;

	if (handle_object_modification(keydata.key, rt->scene) == true)
		rt->scene->render = true;
	else if (keydata.key == MLX_KEY_L)
	{
		light = rt->scene->l.lights;
		idx = 0;
		while (idx < rt->scene->l.l_arr_size)
		{
			light[idx].u.l.intersect_lights = !light[idx].u.l.intersect_lights;
			++idx;
		}
		rt->scene->render = true;
	}
	else if (keydata.key == MLX_KEY_RIGHT_SHIFT && THREADS > 1)
	{
		rt->scene->soft_shadows = !rt->scene->soft_shadows;
		rt->scene->render = true;
	}
	else
		brightness_keys(keydata, rt);
}

static void	closing_cleanup_threads(t_rt *rt)
{
	pthread_mutex_lock(rt->mtx + MTX_QUIT_ROUTINE);
	pthread_mutex_unlock(rt->mtx + MTX_RESYNC);
	rt->quit_routine = true;
	pthread_mutex_unlock(rt->mtx + MTX_QUIT_ROUTINE);
	pthread_mutex_lock(rt->mtx + MTX_STOPPED_THREADS);
	while (rt->stopped_threads < THREADS - 1)
	{
		pthread_cond_wait(&rt->cond, rt->mtx + MTX_STOPPED_THREADS);
	}
	pthread_mutex_unlock(rt->mtx + MTX_STOPPED_THREADS);
}
