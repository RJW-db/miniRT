#include "scene.h"

//	Static Functions
static void	plane_line(t_objs *pl, t_dbltoa *dbl, int fd);
static void	sphere_line(t_objs *sp, t_dbltoa *dbl, int fd);
static void	cylinder_line(t_objs *cy, t_dbltoa *dbl, int fd);

void	objs_line(t_objs *objs, size_t amount, t_dbltoa *dbl, int fd)
{
	size_t	i;

	i = 0;
	while (i < amount)
	{
		if (objs[i].type == PLANE)
			plane_line(objs + i, dbl, fd);
		else if (objs[i].type == SPHERE)
			sphere_line(objs + i, dbl, fd);
		else if (objs[i].type == CYLINDER)
			cylinder_line(objs + i, dbl, fd);
		++i;
	}
}

static void	plane_line(t_objs *pl, t_dbltoa *dbl, int fd)
{
	char	rt_line[RT_MAX_LINE_LEN];
	size_t	line_index;

	line_index = cpy_str0(rt_line, "pl\t");
	line_index += coords_line(dbl, rt_line + line_index, pl->coords);
	line_index += coords_line(dbl, rt_line + line_index, pl->u.plane.orientation);
	line_index += color_line(dbl, rt_line + line_index, pl->color);
	ft_putendl_fd(rt_line, fd);
}

static void	sphere_line(t_objs *sp, t_dbltoa *dbl, int fd)
{
	char	rt_line[RT_MAX_LINE_LEN];
	size_t	line_index;

	line_index = cpy_str0(rt_line, "sp\t");
	line_index += coords_line(dbl, rt_line + line_index, sp->coords);
	dbl->value = sp->u.sphere.radius * 2;
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	line_index += cpy_str0(rt_line + line_index, "\t\t");
	line_index += color_line(dbl, rt_line + line_index, sp->color);
	ft_putendl_fd(rt_line, fd);
}

static void	cylinder_line(t_objs *cy, t_dbltoa *dbl, int fd)
{
	char	rt_line[RT_MAX_LINE_LEN];
	size_t	line_index;

	line_index = cpy_str0(rt_line, "cy\t");
	line_index += coords_line(dbl, rt_line + line_index, cy->coords);
	line_index += coords_line(dbl, rt_line + line_index, cy->u.cylinder.orientation);
	dbl->value = cy->u.cylinder.radius * 2;
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	line_index += cpy_str0(rt_line + line_index, "\t\t");
	dbl->value = cy->u.cylinder.height;
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	line_index += cpy_str0(rt_line + line_index, "\t\t");
	line_index += color_line(dbl, rt_line + line_index, cy->color);
	ft_putendl_fd(rt_line, fd);
}
