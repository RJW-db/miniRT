#include <scene.h>

//	Static Functions
static void	print_camera(t_objs cam);
static void	print_ambient(t_objs amb);
static void	print_light(t_objs l);

void	print_obj_info(t_scene *sc)
{
	static void	(*print_lights[])(t_objs) = {
		print_light,
		print_ambient
	};

	if (sc->selected_obj == NULL)
	{
		print_camera(sc->camera);
	}
	else
	{
		if (sc->selected_obj->type <= CYLINDER)
			geometric_primitives(sc->selected_obj);
		else
			print_lights[sc->selected_obj->type - LIGHT](*sc->selected_obj);
	}
}

static void	print_camera(t_objs cam)
{
	printf("\nCamera\nCoordinates: (x=%f, y=%f, z=%f)\n  "
		"Direction: (%f, %f, %f)\n        "
		"Fov: %f\n"
		"realtimeFov: %f\n   "
		"zvp_dist: %f\n",
		cam.coords[X], cam.coords[Y], cam.coords[Z],
		cam.u.c.orientation[X], cam.u.c.orientation[Y], cam.u.c.orientation[Z],
		cam.u.c.fov, cam.u.c.fov, cam.u.c.zvp_dist);
}

static void	print_ambient(t_objs amb)
{
	printf("\nAmbient\n     "
		"Ratio: %f\n    "
		"Colour: (%d, %d, %d)\n",
		amb.u.a.ratio,
		(int)(amb.color[R] * 255), (int)(amb.color[G] * 255),
		(int)(amb.color[B] * 255));
}

static void	print_light(t_objs l)
{
	printf("\nLight\nCoordinates: (x=%f, y=%f, z=%f)\n      "
		"Ratio:	%f)\n      "
		"Radius: %f\n      "
		"Colour: (%d, %d, %d)\n",
		l.coords[X], l.coords[Y], l.coords[Z],
		l.u.a.ratio, l.u.l.radius,
		(int)(l.color[R] * 255), (int)(l.color[G] * 255),
		(int)(l.color[B] * 255));
}
