#include <parsing.h>
#include <mathRT.h>

//	Static Functions
static bool	parse_amb(t_objs *ambient, t_value_check *vc, char *line);
static bool	parse_cam(t_objs *camera, t_value_check *vc, char *line);
static bool	parse_light(t_value_check *vc, char *line);
static bool	parse_light_extra(t_objs *l, char **line);

bool	input_type_parse(t_scene *sc, t_value_check *vc, char *line)
{
	if (ft_strncmp(line, "A", 1) == 0)
		return (parse_amb(&sc->ambient, vc, nxtv(line)));
	else if (ft_strncmp(line, "C", 1) == 0)
		return (parse_cam(&sc->camera, vc, nxtv(line)));
	else if (ft_strncmp(line, "L", 1) == 0)
		return (parse_light(vc, nxtv(line)));
	return (init_primitives(vc, line));
}

static bool	parse_amb(t_objs *ambient, t_value_check *vc, char *line)
{
	ambient->type = AMBIENT;
	ambient->u.a.ratio = rt_atof(line);
	if ((ambient->u.a.ratio < 0.0F || ambient->u.a.ratio > 1.0F) ||
	validate_and_normalize_color(&ambient->color, &line) == false)
		return (errset(perr("parse_amb", ERRFORM)), EXIT_FAILURE);
	++vc->amb_amount;
	return (EXIT_SUCCESS);
}


static bool	parse_cam(t_objs *camera, t_value_check *vc, char *line)
{
	camera->coords[X] = rt_atof(line);
	camera->coords[Y] = rt_atof(nxtvp(&line));
	camera->coords[Z] = rt_atof(nxtvp(&line));
	if (validate_orientation(&camera->u.c.orientation, &line) == false)
		return (errset(perr("parse_cam", ERRFORM)), EXIT_FAILURE);
	camera->u.c.orientation = vnorm(camera->u.c.orientation);
	camera->u.c.fov = atoi32(nxtvp(&line));
	if (camera->u.c.fov < 0 || camera->u.c.fov > 180)
		return (errset(perr("parse_cam", ERRFORM)), EXIT_FAILURE);
	camera->u.c.zvp_dist = 1.0F / tanf((camera->u.c.fov * M_PI / 180.0F) / 2.0F);
	++vc->cam_amount;
	return (EXIT_SUCCESS);
}

static bool	parse_light(t_value_check *vc, char *line)
{
	t_objs	l;

	l.type = LIGHT;
	l.coords[X] = rt_atof(line);
	l.coords[Y] = rt_atof(nxtvp(&line));
	l.coords[Z] = rt_atof(nxtvp(&line));
	l.coords[W] = 1.0F;
	l.u.l.brightness = rt_atof(nxtvp(&line));
	if ((l.u.l.brightness < 0.0F || l.u.l.brightness > 1.0F) ||
	validate_and_normalize_color(&l.color, &line) == false)
		return (errset(perr("parse_light", ERRFORM)), EXIT_FAILURE);
	l.u.l.radius = 1.5F;
	l.u.l.intersect_lights = false;
	l.u.l.visible = false;
	if (parse_light_extra(&l, &line) == false)
		return (errset(perr("parse_light", ERRFORM)), EXIT_FAILURE);
	if (dynarr_insert(&vc->light_dynarr, &l) == false)
		return (errset(perr("parse_light", ENOMEM)));
	return (EXIT_SUCCESS);
}

static bool	parse_light_extra(t_objs *l, char **line)
{
	while (ft_isnum(*line) == true)
		++(*line);
	while (ft_isspace(**line) == true)
		++(*line);
	if (ft_isnum(*line) == true)
	{
		l->u.l.radius = rt_atof(*line);
		if (l->u.l.radius < 0 || validate_and_normalize_color(&l->u.l.obj_color, line) == false)
			return (false);
		l->u.l.visible = true;
	}
	return (true);
}
