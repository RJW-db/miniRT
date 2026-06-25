#include "RTmlx.h"
#include "render.h"

//	Static Functions
static void	obj_hook(t_rt *rt);
static void	cam_hook(t_rt *rt);

void	movement(t_rt *rt)
{
	if (rt->win->file_creation == false)
	{
		if (rt->scene->selected_obj != NULL)
		{
			obj_hook(rt);
		}
		else
		{
			cam_hook(rt);
		}
	}
}

static void	obj_hook(t_rt *rt)
{
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_W))
		rt->scene->render = obj_move_forw(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_S))
		rt->scene->render = obj_move_backw(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_A))
		rt->scene->render = obj_move_left(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_D))
		rt->scene->render = obj_move_right(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_SPACE))
		rt->scene->render = obj_move_up(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_LEFT_SHIFT))
		rt->scene->render = obj_move_down(rt->scene);
	if (rt->scene->selected_obj->type != SPHERE)
	{
		if (mlx_is_key_down(rt->win->mlx, MLX_KEY_LEFT))
			obj_rotate_left(rt->scene);
		if (mlx_is_key_down(rt->win->mlx, MLX_KEY_RIGHT))
			obj_rotate_right(rt->scene);
		if (mlx_is_key_down(rt->win->mlx, MLX_KEY_UP))
			obj_rotate_up(rt->scene);
		if (mlx_is_key_down(rt->win->mlx, MLX_KEY_DOWN))
			obj_rotate_down(rt->scene);
	}
}

static void	cam_hook(t_rt *rt)
{
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_W))
		cam_move_forw(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_S))
		cam_move_backw(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_D))
		cam_move_right(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_A))
		cam_move_left(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_SPACE))
		cam_move_up(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_LEFT_SHIFT))
		cam_move_down(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_LEFT))
		cam_rotate_left(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_RIGHT))
		cam_rotate_right(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_UP))
		cam_rotate_up(rt->scene);
	if (mlx_is_key_down(rt->win->mlx, MLX_KEY_DOWN))
		cam_rotate_down(rt->scene);
}
