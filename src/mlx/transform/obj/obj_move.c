#include <scene.h>
#include <mathRT.h>
#include <RTmlx.h>

bool	obj_move_forw(t_scene *sc)
{
	sc->selected_obj->coords += sc->camera.u.c.orientation * bcast3(sc->camera.u.c.cam_m_speed);
	return (true);
}

bool	obj_move_backw(t_scene *sc)
{
	sc->selected_obj->coords -= sc->camera.u.c.orientation * bcast3(sc->camera.u.c.cam_m_speed);
	return (true);
}

bool	obj_move_left(t_scene *sc)
{
	const t_vec4	speed = bcast3(sc->camera.u.c.cam_m_speed);
	const t_vec4	left = vnorm(vcross(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F}));

	sc->selected_obj->coords += left * speed;
	return (true);
}

bool	obj_move_right(t_scene *sc)
{
	const t_vec4	speed = bcast3(sc->camera.u.c.cam_m_speed);
	const t_vec4	right = vnorm(vcross(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F}));

	sc->selected_obj->coords -= right * speed;
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