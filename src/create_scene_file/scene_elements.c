#include "scene.h"

void	ambient_line(t_objs *ambient, t_dbltoa *dbl, int fd)
{
	char	buf[RT_MAX_LINE_LEN];
	size_t	line_idx;

	line_idx = cpy_str0(buf, "A\t");
	dbl->value = ambient->u.a.ratio;
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	line_idx += cpy_str0(buf + line_idx, "\t\t");
	line_idx += color_line(dbl, buf + line_idx, ambient->color);
	ft_putendl_fd(buf, fd);
}

void	camera_line(t_objs *camera, t_dbltoa *dbl, int fd)
{
	char	buf[RT_MAX_LINE_LEN];
	size_t	line_idx;

	line_idx = cpy_str0(buf, "C\t");
	line_idx += coords_line(dbl, buf + line_idx, camera->coords);
	line_idx += coords_line(dbl, buf + line_idx, camera->u.c.orientation);
	dbl->value = camera->u.c.fov;
	dbl->precision = 0;
	dbltoa_buff_prec(*dbl);
	cpy_str0(buf + line_idx, dbl->buff);
	ft_putendl_fd(buf, fd);
}

void	lights_line(t_objs *lights, size_t amount, t_dbltoa *dbl, int fd)
{
	char	buf[RT_MAX_LINE_LEN];
	size_t	line_idx;
	size_t	i;

	i = 0;
	while (i < amount)
	{
		line_idx = cpy_str0(buf, "L\t");
		line_idx += coords_line(dbl, buf + line_idx, lights[i].coords);
		dbl->value = lights[i].u.l.brightness;
		dbl->precision = 1;
		dbltoa_buff_prec(*dbl);
		line_idx += cpy_str0(buf + line_idx, dbl->buff);
		line_idx += cpy_str0(buf + line_idx, "\t\t");
		line_idx += color_line(dbl, buf + line_idx, lights[i].color);
		if (lights[i].u.l.visible == true)
		{
			dbl->value = lights[i].u.l.radius;
			dbltoa_buff_prec(*dbl);
			line_idx += cpy_srcs(buf + line_idx, "\t\t", dbl->buff, "\t\t");
			line_idx += color_line(dbl, buf + line_idx, lights[i].color);
		}
		ft_putendl_fd(buf, fd);
		++i;
	}
}
