#include "RTmlx.h"
#include "scene.h"

#define RADIUS true
#define HEIGHT false

//	Static Functions
static void	select_obj_left(t_scene *sc);
static void	select_obj_right(t_scene *sc);
static bool	increase_obj(t_objs *selected_obj, bool radius_or_height);
static bool	decrease_obj(t_objs *selected_obj, bool radius_or_height);

bool	handle_object_modification(const keys_t key, t_scene *sc)
{
	if (key == MLX_KEY_V)
		select_obj_left(sc);
	else if (key == MLX_KEY_N)
		select_obj_right(sc);
	else if (key == MLX_KEY_B)
		sc->selected_obj = NULL;
	else if (key == MLX_KEY_M)
		sc->render = decrease_obj(sc->selected_obj, HEIGHT);
	else if (key == MLX_KEY_COMMA)
		sc->render = decrease_obj(sc->selected_obj, RADIUS);
	else if (key == MLX_KEY_PERIOD)
		sc->render = increase_obj(sc->selected_obj, RADIUS);
	else if (key == MLX_KEY_SLASH)
		sc->render = increase_obj(sc->selected_obj, HEIGHT);
	else
		return (false);
	return (true);
}

static void	select_obj_left(t_scene *sc)
{
	ssize_t	index;

	index = sc->sel_obj_index - 1;
	while (index >= 0 && sc->o.objs[index].type == PLANE)
		--index;
	if (index < 0)
	{
		index = (ssize_t)(sc->o.o_arr_size - 1);
		while (index >= 0 && sc->o.objs[index].type == PLANE)
			--index;
		if (index < 0)
			return ;
	}
	sc->selected_obj = sc->o.objs + index;
	sc->sel_obj_index = index;
}

static void	select_obj_right(t_scene *sc)
{
	ssize_t	obj_arr_size;
	ssize_t	index;

	index = sc->sel_obj_index + 1;
	obj_arr_size = (ssize_t)sc->o.o_arr_size;
	while (index < obj_arr_size && sc->o.objs[index].type == PLANE)
		++index;
	if (index >= obj_arr_size)
	{
		index = 0;
		while (index < obj_arr_size && sc->o.objs[index].type == PLANE)
			++index;
		if (index >= obj_arr_size)
			return ;
	}
	sc->selected_obj = sc->o.objs + index;
	sc->sel_obj_index = index;
}

static bool	increase_obj(t_objs *selected_obj, bool radius_or_height)
{
	if (selected_obj == NULL)
		return (false);
	if (selected_obj->type == SPHERE)
		selected_obj->u.sphere.radius *= 1.1F;
	else if (selected_obj->type == LIGHT)
		selected_obj->u.l.radius *= 1.1F;
	else if (radius_or_height == RADIUS)
		selected_obj->u.cylinder.radius *= 1.1F;
	else if (radius_or_height == HEIGHT)
		selected_obj->u.cylinder.height *= 1.1F;
	else
		return (false);
	return (true);
}

static bool	decrease_obj(t_objs *selected_obj, bool radius_or_height)
{
	union u_union	*obj_data;

	if (selected_obj == NULL)
		return (false);
	obj_data = &selected_obj->u;
	if (selected_obj->type == SPHERE && obj_data->sphere.radius > 0.01F)
		obj_data->sphere.radius /= 1.1F;
	else if (selected_obj->type == LIGHT && obj_data->l.radius > 0.01F)
		obj_data->l.radius /= 1.1F;
	else if (radius_or_height == RADIUS && obj_data->cylinder.radius > 0.01F)
		obj_data->cylinder.radius /= 1.1F;
	else if (radius_or_height == HEIGHT && obj_data->cylinder.height > 0.01F)
		obj_data->cylinder.height /= 1.1F;
	else
		return (false);
	return (true);
}
