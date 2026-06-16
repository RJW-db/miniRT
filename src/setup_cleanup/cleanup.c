#include <setup_clean.h>

//	Static Function
static void	thread_related_clean_up(t_rt *rt);

int32_t	cleanup(t_rt *rt)
{
	ft_putchar_fd('\n', STDOUT_FILENO);
	if (THREADS > 1)
		thread_related_clean_up(rt);
	else if (rt->win->img != NULL)
		mlx_delete_image(rt->win->mlx, rt->win->img);
	if (rt->win->mlx != NULL)
		mlx_terminate(rt->win->mlx);
	if (rt->scene->o.objs)
		free(rt->scene->o.objs);
	if (rt->scene->l.lights)
		free(rt->scene->l.lights);
	return (rt->errnum);
}

static void	thread_related_clean_up(t_rt *rt)
{
	if (rt->creation_check == true)
		destroy_threads(rt);
	if (rt->stopped_threads == THREADS - 1)
		destroy_conditions(rt);
	if (rt->mtx_init_check == true)
		destroy_mutexes(rt, MTX_AMOUNT);
	if (rt->read_scene->o.objs)
		free(rt->read_scene->o.objs);
	if (rt->read_scene->l.lights)
		free(rt->read_scene->l.lights);
	if (rt->thread.img)
		rt->thread.img->pixels = rt->thread.pixels_mlx;
	if (rt->thread.pixels_own)
		free(rt->thread.pixels_own);
	if (rt->thread.img != NULL)
		mlx_delete_image(rt->win->mlx, rt->thread.img);
}
