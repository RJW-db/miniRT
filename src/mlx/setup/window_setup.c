#include <RTmlx.h>

//	Static Functions
static bool	init_mlx(t_window *win);
static bool	img_to_window(t_window *win);
static bool	windows_logo(t_window *win);
static void	center_window(t_window *win);

bool	windows_setup_mlx(t_rt *rt)
{
	if (init_mlx(rt->win) == false || \
		windows_logo(rt->win) == false)
		return (EXIT_FAILURE);
	res_setscale(rt->win, rt->win->res_r_start);
	if (THREADS > 1)
	{
		if (img_multithreaded(rt) == false)
			return (EXIT_FAILURE);
	}
	else
	{
		if (img_to_window(rt->win) == false)
			return (EXIT_FAILURE);
	}
	center_window(rt->win);
	mlx_set_window_title(rt->win->mlx, "Rendering miniRT");
	return (EXIT_SUCCESS);
}

static bool	init_mlx(t_window *win)
{
	int32_t	win_width;
	int32_t	win_height;

	win_width = SCREEN_WIDTH / 2;
	win_height = SCREEN_HEIGHT / 2;
	win->aspectrat = (float)win_width / (float)win_height;
	win->mlx = mlx_init(win_width, win_height, "miniRT is setting up MLX", false);
	if (win->mlx == NULL)
		return (false);
	win->window_hght = (uint16_t)win_height;
	win->window_wdth = (uint16_t)win_width;
	return (true);
}

static bool	img_to_window(t_window *win)
{
	win->img = mlx_new_image(win->mlx, win->window_wdth, win->window_hght);
	if (win->img == NULL)
		return (false);
	win->id = mlx_image_to_window(win->mlx, win->img, 0, 0);
	if (win->id == -1)
		return (mlx_delete_image(win->mlx, win->img), false);
	win->pixels = win->img->pixels;
	return (true);
}

static bool	windows_logo(t_window *win)
{
	mlx_texture_t	*logo_texture;

	logo_texture = mlx_load_png(LOGO_PATH);
	if (logo_texture == NULL)
		return (false);
	mlx_set_icon(win->mlx, logo_texture);
	mlx_delete_texture(logo_texture);
	return (true);
}

static void	center_window(t_window *win)
{
	int32_t	width;
	int32_t	height;

	mlx_get_monitor_size(0, &width, &height);
	width = (width - (int32_t)win->window_wdth) / 2;
	height = (height - (int32_t)win->window_hght) / 2;
	mlx_set_window_pos(win->mlx, width, height);
}
