#include <RTmlx.h>

bool	res_setscale(t_window *win, const float scale)
{
	win->res_ratio = scale;
	if (win->res_ratio < 1.0F)
		win->res_ratio = 1.0F;
	win->rndr_hght = (float)win->mlx->height / win->res_ratio;
	win->rndr_wdth = (float)win->mlx->width / win->res_ratio;
	return (true);
}

bool	img_multithreaded(t_rt *rt)
{
	const uint16_t	width = (uint32_t)rt->win->window_wdth;
	const uint16_t	height = (uint32_t)rt->win->window_hght;

	rt->thread.img = mlx_new_image(rt->win->mlx, width, height);
	if (rt->thread.img == NULL)
	{
		return (false);
	}
	rt->win->id = mlx_image_to_window(rt->win->mlx, rt->thread.img, 0, 0);
	if (rt->win->id == -1)
	{
		return (mlx_delete_image(rt->win->mlx, rt->thread.img), false);
	}
	rt->thread.pixels_mlx = (uint8_t *)rt->thread.img->pixels;
	rt->thread.pixels_own = (uint8_t *)malloc(sizeof(uint8_t) * (size_t)width * (size_t)height * 4U);
	if (rt->thread.pixels_own == NULL)
		return (mlx_delete_image(rt->win->mlx, rt->thread.img), false);
	rt->thread.pixels = rt->thread.pixels_own;
	return (true);
}
