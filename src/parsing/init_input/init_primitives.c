#include <parsing.h>
#include <mathRT.h>

//	Static Functions
static bool		parse_pl(t_value_check *vc, char *line);
static bool		parse_sp(t_value_check *vc, char *line);
static bool		parse_cy(t_value_check *vc, char *line);

bool	init_primitives(t_value_check *vc, char *line)
{
	if (ft_strncmp(line, "pl", 2) == 0)
		return (parse_pl(vc, nxtv(line)));
	else if (ft_strncmp(line, "sp", 2) == 0)
		return (parse_sp(vc, nxtv(line)));
	return (parse_cy(vc, nxtv(line)));
}

static bool	parse_pl(t_value_check *vc, char *line)
{
	t_objs	pl;

	pl.type = PLANE;
	pl.coords[X] = rt_atof(line);
	pl.coords[Y] = rt_atof(nxtvp(&line));
	pl.coords[Z] = rt_atof(nxtvp(&line));
	pl.coords[W] = 1.0F;
	if (validate_orientation(&pl.u.plane.orientation, &line) == false)
		return (errset(perr("parse_pl", ERRFORM)), EXIT_FAILURE);
	if (validate_and_normalize_color(&pl.color, &line) == false)
		return (errset(perr("parse_pl", ERRFORM)), EXIT_FAILURE);
	if (dynarr_insert(&vc->obj_dynarr, &pl) == false)
		return (errset(perr("parse_pl", ENOMEM)));
	return (EXIT_SUCCESS);
}

static bool	parse_sp(t_value_check *vc, char *line)
{
	t_objs	sp;

	sp.type = SPHERE;
	sp.coords[X] = rt_atof(line);
	sp.coords[Y] = rt_atof(nxtvp(&line));
	sp.coords[Z] = rt_atof(nxtvp(&line));
	sp.coords[W] = 1.0F;
	sp.u.sphere.diameter = rt_atof(nxtvp(&line));
	if (sp.u.sphere.diameter < 0)
		return (errset(perr("parse_sp", ERRFORM)), EXIT_FAILURE);
	sp.u.sphere.radius = sp.u.sphere.diameter / 2.0F;
	if (validate_and_normalize_color(&sp.color, &line) == false)
		return (errset(perr("parse_sp", ERRFORM)), EXIT_FAILURE);
	if (dynarr_insert(&vc->obj_dynarr, &sp) == false)
		return (errset(perr("parse_sp", ENOMEM)));
	return (EXIT_SUCCESS);
}

static bool	parse_cy(t_value_check *vc, char *line)
{
	t_objs	cy;

	cy.type = CYLINDER;
	cy.coords[X] = rt_atof(line);
	cy.coords[Y] = rt_atof(nxtvp(&line));
	cy.coords[Z] = rt_atof(nxtvp(&line));
	cy.coords[W] = 1.0F;
	if (validate_orientation(&cy.u.cylinder.orientation, &line) == false)
		return (errset(perr("parse_cy", ERRFORM)), EXIT_FAILURE);
	cy.u.cylinder.diameter = rt_atof(nxtvp(&line));
	if (cy.u.cylinder.diameter < 0)
		return (errset(perr("parse_cy", ERRFORM)), EXIT_FAILURE);
	cy.u.cylinder.radius = cy.u.cylinder.diameter / 2.0F;
	cy.u.cylinder.height = rt_atof(nxtvp(&line));
	if (cy.u.cylinder.height < 0)
		return (errset(perr("parse_cy", ERRFORM)), EXIT_FAILURE);
	if (validate_and_normalize_color(&cy.color, &line) == false)
		return (errset(perr("parse_cy", ERRFORM)), EXIT_FAILURE);
	if (dynarr_insert(&vc->obj_dynarr, &cy) == false)
		return (errset(perr("parse_cy", ENOMEM)));
	return (EXIT_SUCCESS);
}
