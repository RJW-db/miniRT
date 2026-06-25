#include "scene.h"
#include "mathRT.h"
#include "RTmlx.h"

void	obj_rotate_up(t_scene *sc)
{
	t_vec4	right;
	t_objs	*picked;
	t_vec4	*orientation;
	float	camera_speed;

	right = vcross(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F});
	picked = sc->selected_obj;
	camera_speed = sc->camera.u.c.cam_r_speed;
	if (picked->type == PLANE)
	{
		orientation = &picked->u.plane.orientation;
		*orientation = vnorm(vrotate(*orientation, right, camera_speed));
	}
	else if (picked->type == CYLINDER)
	{
		orientation = &picked->u.cylinder.orientation;
		*orientation = vnorm(vrotate(*orientation, right, camera_speed));
	}
	sc->render = true;
}

void	obj_rotate_down(t_scene *sc)
{
	t_vec4	right;
	t_objs	*picked;
	t_vec4	*orientation;
	float	camera_speed;

	right = vcross(sc->camera.u.c.orientation, (t_vec4){0.0F, 1.0F, 0.0F});
	picked = sc->selected_obj;
	camera_speed = sc->camera.u.c.cam_r_speed;
	if (picked->type == PLANE)
	{
		orientation = &picked->u.plane.orientation;
		*orientation = vnorm(vrotate(*orientation, right, -camera_speed));
	}
	else if (picked->type == CYLINDER)
	{
		orientation = &picked->u.cylinder.orientation;
		*orientation = vnorm(vrotate(*orientation, right, -camera_speed));
	}
	sc->render = true;
}

void	obj_rotate_left(t_scene *sc)
{
	t_vec4	rotation_axis;
	t_objs	*picked;
	t_vec4	*orientation;
	float	camera_speed;

	rotation_axis = (t_vec4){0.0F, 1.0F, 0.0F};
	picked = sc->selected_obj;
	camera_speed = sc->camera.u.c.cam_r_speed;
	if (picked->type == PLANE)
	{
		orientation = &picked->u.plane.orientation;
		*orientation = vnorm(vrotate(*orientation, rotation_axis, -camera_speed));
	}
	else if (picked->type == CYLINDER)
	{
		orientation = &picked->u.cylinder.orientation;
		*orientation = vnorm(vrotate(*orientation, rotation_axis, -camera_speed));
	}
	sc->render = true;
}

void	obj_rotate_right(t_scene *sc)
{
	t_vec4	rotation_axis;
	t_objs	*picked;
	t_vec4	*orientation;
	float	camera_speed;

	rotation_axis = (t_vec4){0.0F, 1.0F, 0.0F};
	picked = sc->selected_obj;
	camera_speed = sc->camera.u.c.cam_r_speed;
	if (picked->type == PLANE)
	{
		orientation = &picked->u.plane.orientation;
		*orientation = vnorm(vrotate(*orientation, rotation_axis, camera_speed));
	}
	else if (picked->type == CYLINDER)
	{
		orientation = &picked->u.cylinder.orientation;
		*orientation = vnorm(vrotate(*orientation, rotation_axis, camera_speed));
	}
	sc->render = true;
}
