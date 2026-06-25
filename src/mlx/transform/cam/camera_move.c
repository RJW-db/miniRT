#include "scene.h"
#include "mathRT.h"

void	cam_move_forw(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const float		move_speed = sc->camera.u.c.cam_m_speed;
	const t_vec4	movement = forward * bcast3(move_speed);

	sc->camera.coords += movement;
	sc->render = true;
}

void	cam_move_backw(t_scene *sc)
{
	const t_vec4	backward = -sc->camera.u.c.orientation;
	const float		move_speed = sc->camera.u.c.cam_m_speed;
	const t_vec4	movement = backward * bcast3(move_speed);

	sc->camera.coords += movement;
	sc->render = true;
}

void	cam_move_right(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const t_vec4	right = vnorm(vcross(forward, world_up));
	const float		move_speed = sc->camera.u.c.cam_m_speed;
	const t_vec4	movement = right * bcast3(move_speed);

	sc->camera.coords -= movement;
	sc->render = true;
}

void	cam_move_left(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const t_vec4	left = vnorm(vcross(forward, world_up));
	const float		move_speed = sc->camera.u.c.cam_m_speed;
	const t_vec4	movement = left * bcast3(move_speed);

	sc->camera.coords += movement;
	sc->render = true;
}

void	cam_move_up(t_scene *sc)
{
	sc->camera.coords[Y] += sc->camera.u.c.cam_m_speed;
	sc->render = true;
}

void	cam_move_down(t_scene *sc)
{
	sc->camera.coords[Y] -= sc->camera.u.c.cam_m_speed;
	sc->render = true;
}
