#include <RTmlx.h>
#include <render.h>
#include <mathRT.h>

//	Static Functions
static void	closing_cleanup_threads(t_rt *rt);
static void	scene_manipulate_keys(mlx_key_data_t keydata, t_rt *rt);

void	init_hooks(t_rt *rt)
{
	if (THREADS > 1)
	{
		mlx_loop_hook(rt->win->mlx, (mlx_closefunc)loop_hook_threaded, rt);
		mlx_close_hook(rt->win->mlx, (mlx_closefunc)closing_cleanup_threads, rt);
	}
	else
		mlx_loop_hook(rt->win->mlx, (mlx_closefunc)loop_hook, rt);
	mlx_key_hook(rt->win->mlx, (mlx_keyfunc)my_keyhook, rt);
	mlx_scroll_hook(rt->win->mlx, (mlx_cursorfunc)scroll_fov_hook, rt->scene);
	mlx_mouse_hook(rt->win->mlx, (mlx_mousefunc)mouse_hook, rt);
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
	float		upl;
	float		dol;
	const float	upa = rt->scene->ambient.u.a.ratio + 0.05F;
	const float	doa = rt->scene->ambient.u.a.ratio - 0.05F;

	if (keydata.key == MLX_KEY_EQUAL)
	{
		if (rt->scene->selected_obj != NULL && rt->scene->selected_obj->type == LIGHT)
		{
			upl = rt->scene->selected_obj->u.l.brightness + 0.05F;
			rt->scene->selected_obj->u.l.brightness = clamp(upl, 0.0F, 1.0F);
		}
		else
			rt->scene->ambient.u.a.ratio = clamp(upa, 0.0F, 1.0F);
		rt->scene->render = true;
	}
	else if (keydata.key == MLX_KEY_MINUS)
	{
		if (rt->scene->selected_obj != NULL && rt->scene->selected_obj->type == LIGHT)
		{
			dol = rt->scene->selected_obj->u.l.brightness - 0.05F;
			rt->scene->selected_obj->u.l.brightness = clamp(dol, 0.0F, 1.0F);
		}
		else
			rt->scene->ambient.u.a.ratio = clamp(doa, 0.0F, 1.0F);
		rt->scene->render = true;
	}
}

static void	scene_manipulate_keys(mlx_key_data_t keydata, t_rt *rt)
{
	uint16_t	index;
	t_objs		*light;

	if (handle_object_modification(keydata.key, rt->scene) == true)
		rt->scene->render = true;
	else if (keydata.key == MLX_KEY_L)
	{
		light = rt->scene->l.lights;
		index = 0;
		while (index < rt->scene->l.l_arr_size)
		{
			light[index].u.l.intersect_lights = !light[index].u.l.intersect_lights;
			++index;
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
