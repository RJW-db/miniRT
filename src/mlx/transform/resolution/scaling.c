#include "RTmlx.h"
#include "render.h"
#include "mathRT.h"

void	switch_scaling_mode(t_rt *rt)
{
	pthread_mutex_lock(&rt->mtx[MTX_SWITCH_BOOL]);
	rt->win->auto_res = !rt->win->auto_res;
	pthread_mutex_unlock(&rt->mtx[MTX_SWITCH_BOOL]);
	rt->scene->render = true;
}

void	change_scaling_start(t_rt *rt, mlx_key_data_t keydata)
{
	const uint16_t	lower_val = rt->win->man_r_start - 1;
	const uint16_t	higher_val = rt->win->man_r_start + 1;

	if (keydata.key == MLX_KEY_2)
	{
		rt->win->man_r_start = (uint16_t)intclamp(lower_val, 2, 30);
	}
	if (keydata.key == MLX_KEY_3)
	{
		rt->win->man_r_start = (uint16_t)intclamp(higher_val, 2, 30);
	}
	if (rt->win->auto_res == false)
		rt->scene->render = true;
}
