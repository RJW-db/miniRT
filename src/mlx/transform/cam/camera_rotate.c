#include <scene.h>
#include <mathRT.h>

void	cam_rotate_up(t_scene *sc)
{
	const t_vec4	right = vcross(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F});

	sc->camera.u.c.orientation = vnorm( \
	vrotate(sc->camera.u.c.orientation, right, sc->camera.u.c.cam_r_speed));
	sc->render = true;
}

void	cam_rotate_down(t_scene *sc)
{
	const t_vec4	right = vcross(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F});

	sc->camera.u.c.orientation = vnorm( \
	vrotate(sc->camera.u.c.orientation, right, -sc->camera.u.c.cam_r_speed));
	sc->render = true;
}

void	cam_rotate_left(t_scene *sc)
{
	sc->camera.u.c.orientation = vnorm( \
	vrotate(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F}, -sc->camera.u.c.cam_r_speed));
	sc->render = true;
}

void	cam_rotate_right(t_scene *sc)
{
	sc->camera.u.c.orientation = vnorm( \
	vrotate(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F}, sc->camera.u.c.cam_r_speed));
	sc->render = true;
}
