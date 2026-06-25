#include "RTmlx.h"
#include "render.h"

//	Static Functions
static void	render_updates(t_rt *rt);
static void	switch_pixelarray(t_rt *rt);
static void	last_pix_arr_switch(t_rt *rt);

void	loop_hook(t_rt *rt)
{
	t_camera	*camera;
	t_window	*win;
	double		time;

	time = mlx_get_time();
	movement(rt);
	if (rt->scene->render == false && rt->scene->render_ongoing == false)
		return ;
	win = rt->win;
	if (rt->scene->render == true && win->auto_res == false)
		win->res_r_start = win->man_r_start;
	upscale_manager(rt);
	time = mlx_get_time() - time;
	if (win->res_ratio == win->res_r_start)
		win->delta_time = (float)time;
	else if (win->res_ratio == win->res_r_start - 1 && win->auto_res == true)
		set_starting_res_ratio(rt, time);
	camera = &rt->scene->camera.u.c;
	camera->cam_fov_speed = FOV_SCROLL_SPEED * win->delta_time;
	camera->cam_m_speed = CAM_MOVE_SPEED * win->delta_time;
	camera->cam_r_speed = CAM_ROTATION_SPEED * win->delta_time;
	rt->scene->render = false;
}

void	loop_hook_threaded(t_rt *rt)
{
	last_pix_arr_switch(rt);
	movement(rt);
	if (rt->scene->render == true)
		toggle_bool(rt->mtx + MTX_RENDER, &rt->read_scene->render, true);
	pthread_mutex_lock(rt->mtx + MTX_DONE_RENDERING);
	if ((rt->scene->render == true || rt->scene->render_ongoing == true)
		&& rt->finished_rendering == THREADS - 1)
	{
		pthread_mutex_lock(rt->mtx + MTX_SYNC);
		while (rt->finished_rendering != 0)
		{
			pthread_cond_wait(&rt->cond, rt->mtx + MTX_RESYNC);
		}
		toggle_bool(rt->mtx + MTX_RENDER, &rt->read_scene->render, false);
		pthread_mutex_unlock(rt->mtx + MTX_RESYNC);
		pthread_mutex_lock(rt->mtx + MTX_RESYNC);
		render_updates(rt);
		switch_pixelarray(rt);
		pthread_mutex_unlock(rt->mtx + MTX_SYNC);
	}
	pthread_mutex_unlock(rt->mtx + MTX_DONE_RENDERING);
}

static void	render_updates(t_rt *rt)
{
	t_scene	*sc;

	sc = rt->scene;
	if (sc->render == true)
	{
		cpy_scene(rt->scene, rt->read_scene);
		if (rt->win->auto_res == false)
			rt->win->res_r_start = rt->win->man_r_start;
	}
	upscale_manager_thread(rt);
	sc->camera.u.c.cam_fov_speed = FOV_SCROLL_SPEED * rt->win->delta_time;
	sc->camera.u.c.cam_m_speed = CAM_MOVE_SPEED * rt->win->delta_time;
	sc->camera.u.c.cam_r_speed = CAM_ROTATION_SPEED * rt->win->delta_time;
	sc->render = false;
}

static void	switch_pixelarray(t_rt *rt)
{
	if (rt->thread.pixels == rt->thread.pixels_mlx)
	{
		rt->thread.img->pixels = rt->thread.pixels_mlx;
		rt->thread.pixels = rt->thread.pixels_own;
	}
	else
	{
		rt->thread.img->pixels = rt->thread.pixels_own;
		rt->thread.pixels = rt->thread.pixels_mlx;
	}
}

static void	last_pix_arr_switch(t_rt *rt)
{
	static bool	switch_it = false;
	const char	*title = "miniRT is upsampling shadows (can take minutes)";

	pthread_mutex_lock(rt->mtx + MTX_DONE_RENDERING);
	if (rt->scene->render == true)
	{
		switch_it = false;
	}
	else if (rt->scene->render == false
		&& rt->scene->render_ongoing == true
		&& rt->win->res_ratio == RES_R_FULL)
	{
		if (rt->scene->soft_shadows == true)
			mlx_set_window_title(rt->win->mlx, title);
		switch_it = true;
	}
	else if (switch_it == true
		&& rt->scene->render_ongoing == false
		&& rt->finished_rendering == THREADS - 1)
	{
		mlx_set_window_title(rt->win->mlx, "miniRT");
		switch_pixelarray(rt);
		switch_it = false;
	}
	pthread_mutex_unlock(rt->mtx + MTX_DONE_RENDERING);
}
