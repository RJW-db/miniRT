#include "scene.h"
#include "mathRT.h"
#include "RTmlx.h"

bool	obj_move_forw(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const float		move_speed = sc->camera.u.c.cam_m_speed;
	const t_vec4	movement = forward * bcast3(move_speed);

	sc->selected_obj->coords += movement;
	return (true);
}

bool	obj_move_backw(t_scene *sc)
{
	const t_vec4	backward = -sc->camera.u.c.orientation;
	const float		move_speed = sc->camera.u.c.cam_m_speed;
	const t_vec4	movement = backward * bcast3(move_speed);

	sc->selected_obj->coords += movement;
	return (true);
}

bool	obj_move_left(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const t_vec4	left = vnorm(vcross(forward, world_up));
	const float		move_speed = sc->camera.u.c.cam_m_speed;

	sc->selected_obj->coords += left * bcast3(move_speed);
	return (true);
}

bool	obj_move_right(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const t_vec4	right = vnorm(vcross(forward, world_up));
	const float		move_speed = sc->camera.u.c.cam_m_speed;

	sc->selected_obj->coords -= right * bcast3(move_speed);
	return (true);
}

bool	obj_move_up(t_scene *sc)
{
	sc->selected_obj->coords[Y] += sc->camera.u.c.cam_m_speed;
	return (true);
}

bool	obj_move_down(t_scene *sc)
{
	sc->selected_obj->coords[Y] -= sc->camera.u.c.cam_m_speed;
	return (true);
}
