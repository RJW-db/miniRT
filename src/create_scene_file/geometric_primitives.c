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
	char	buf[RT_MAX_LINE_LEN];
	size_t	line_idx;

	line_idx = cpy_str0(buf, "pl\t");
	line_idx += coords_line(dbl, buf + line_idx, pl->coords);
	line_idx += coords_line(dbl, buf + line_idx, pl->u.plane.orientation);
	line_idx += color_line(dbl, buf + line_idx, pl->color);
	ft_putendl_fd(buf, fd);
}

static void	sphere_line(t_objs *sp, t_dbltoa *dbl, int fd)
{
	char	buf[RT_MAX_LINE_LEN];
	size_t	line_idx;

	line_idx = cpy_str0(buf, "sp\t");
	line_idx += coords_line(dbl, buf + line_idx, sp->coords);
	dbl->value = sp->u.sphere.radius * 2;
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	line_idx += cpy_str0(buf + line_idx, "\t\t");
	line_idx += color_line(dbl, buf + line_idx, sp->color);
	ft_putendl_fd(buf, fd);
}

static void	cylinder_line(t_objs *cy, t_dbltoa *dbl, int fd)
{
	char	buf[RT_MAX_LINE_LEN];
	size_t	line_idx;

	line_idx = cpy_str0(buf, "cy\t");
	line_idx += coords_line(dbl, buf + line_idx, cy->coords);
	line_idx += coords_line(dbl, buf + line_idx, cy->u.cylinder.orientation);
	dbl->value = cy->u.cylinder.radius * 2;
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	line_idx += cpy_str0(buf + line_idx, "\t\t");
	dbl->value = cy->u.cylinder.height;
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	line_idx += cpy_str0(buf + line_idx, "\t\t");
	line_idx += color_line(dbl, buf + line_idx, cy->color);
	ft_putendl_fd(buf, fd);
}
