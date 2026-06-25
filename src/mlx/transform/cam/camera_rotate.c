#include "scene.h"
#include "mathRT.h"

void	cam_rotate_up(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const t_vec4	right = vcross(forward, world_up);
	const float		rotation_speed = sc->camera.u.c.cam_r_speed;

	sc->camera.u.c.orientation = vnorm(vrotate(forward, right, rotation_speed));
	sc->render = true;
}

void	cam_rotate_down(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const t_vec4	right = vcross(forward, world_up);
	const float		rotation_speed = sc->camera.u.c.cam_r_speed;

	sc->camera.u.c.orientation = vnorm(vrotate(forward, right, -rotation_speed));
	sc->render = true;
}

void	cam_rotate_left(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const float		rotation_speed = sc->camera.u.c.cam_r_speed;

	sc->camera.u.c.orientation = vnorm(vrotate(forward, world_up, -rotation_speed));
	sc->render = true;
}

void	cam_rotate_right(t_scene *sc)
{
	const t_vec4	forward = sc->camera.u.c.orientation;
	const t_vec4	world_up = (t_vec4){0.0F, 1.0F, 0.0F};
	const float		rotation_speed = sc->camera.u.c.cam_r_speed;

	sc->camera.u.c.orientation = vnorm(vrotate(forward, world_up, rotation_speed));
	sc->render = true;
}
