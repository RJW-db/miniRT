#include "RTmlx.h"
#include "mathRT.h"
#include "render.h"

//	Static functions
static void	set_shadow_gridsize(t_rt *rt, uint8_t stage);

void	upscale_manager(t_rt *rt)
{
	if (rt->scene->render == true)
	{
		rt->win->res_ratio = rt->win->res_r_start;
		rt->scene->render_ongoing = true;
		mlx_set_window_title(rt->win->mlx, "Rendering miniRT");
	}
	else if (rt->win->res_ratio > RES_R_FULL)
	{
		rt->win->res_ratio = (uint16_t)(intclamp( \
			rt->win->res_ratio - RES_STEP_SIZE, RES_R_FULL, rt->win->res_r_start));
	}
	else
	{
		rt->scene->render_ongoing = false;
		mlx_set_window_title(rt->win->mlx, "miniRT");
	}
	rt->win->rndr_hght = (uint32_t)rt->win->mlx->height / rt->win->res_ratio;
	rt->win->rndr_wdth = (uint32_t)rt->win->mlx->width / rt->win->res_ratio;
	render(rt, rt->win);
}

void	upscale_manager_thread(t_rt *rt)
{
	if (rt->scene->render == true)
	{
		if (rt->win->res_ratio == rt->win->res_r_start)
			return ;
		rt->win->res_ratio = rt->win->res_r_start;
		rt->scene->render_ongoing = true;
		set_shadow_gridsize(rt, RSTAGE_START);
		mlx_set_window_title(rt->win->mlx, "Rendering miniRT");
	}
	else if (rt->win->res_ratio > RES_R_FULL)
	{
		rt->win->res_ratio = (uint16_t)(intclamp( \
		rt->win->res_ratio - RES_STEP_SIZE, RES_R_FULL, rt->win->res_r_start));
		set_shadow_gridsize(rt, RSTAGE_ONGOING);
	}
	else
	{
		rt->scene->render_ongoing = false;
		set_shadow_gridsize(rt, RSTAGE_END);
	}
	rt->win->rndr_hght = (uint32_t)rt->win->mlx->height / rt->win->res_ratio;
	rt->win->rndr_wdth = (uint32_t)rt->win->mlx->width / rt->win->res_ratio;
}

void	set_starting_res_ratio(t_rt *rt, double delta_time)
{
	const double	error = delta_time - rt->win->target_time;
	const double	adjustment_factor = 8.0F;
	int				new_ratio;

	new_ratio = (int)(rt->win->res_r_start * (1.0F + (error * adjustment_factor)));
	rt->win->res_r_start = (uint16_t)intclamp((int)new_ratio, 2, 30);
}

static void	set_shadow_gridsize(t_rt *rt, uint8_t stage)
{
	if (stage == RSTAGE_START)
		rt->read_scene->shadow_grsize = 1;
	else if (stage == RSTAGE_ONGOING)
	{
		if (rt->win->res_ratio == 1)
			rt->read_scene->shadow_grsize = 5;
		else if (rt->win->res_ratio < rt->win->res_r_start - 2 \
			&& rt->read_scene->shadow_grsize < 3)
			rt->read_scene->shadow_grsize += rt->win->res_ratio % 2;
	}
	else if (stage == RSTAGE_END)
	{
		rt->read_scene->shadow_grsize = 8;
	}
}
