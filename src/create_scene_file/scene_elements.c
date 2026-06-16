#include <scene.h>

void	ambient_line(t_objs *ambient, t_dbltoa *dbl, int fd)
{
	char	rt_line[RT_MAX_LINE_LEN];
	size_t	line_index;

	line_index = cpy_str0(rt_line, "A\t");
	dbl->value = ambient->u.a.ratio;
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	line_index += cpy_str0(rt_line + line_index, "\t\t");
	line_index += color_line(dbl, rt_line + line_index, ambient->color);
	ft_putendl_fd(rt_line, fd);
}

void	camera_line(t_objs *camera, t_dbltoa *dbl, int fd)
{
	char	rt_line[RT_MAX_LINE_LEN];
	size_t	line_index;

	line_index = cpy_str0(rt_line, "C\t");
	line_index += coords_line(dbl, rt_line + line_index, camera->coords);
	line_index += coords_line(dbl, rt_line + line_index, camera->u.c.orientation);
	dbl->value = camera->u.c.fov;
	dbl->precision = 0;
	dbltoa_buff_prec(*dbl);
	cpy_str0(rt_line + line_index, dbl->buff);
	ft_putendl_fd(rt_line, fd);
}

void	lights_line(t_objs *lights, size_t amount, t_dbltoa *dbl, int fd)
{
	char	rt_line[RT_MAX_LINE_LEN];
	size_t	line_index;
	size_t	i;

	i = 0;
	while (i < amount)
	{
		line_index = cpy_str0(rt_line, "L\t");
		line_index += coords_line(dbl, rt_line + line_index, lights[i].coords);
		dbl->value = lights[i].u.l.brightness;
		dbl->precision = 1;
		dbltoa_buff_prec(*dbl);
		line_index += cpy_str0(rt_line + line_index, dbl->buff);
		line_index += cpy_str0(rt_line + line_index, "\t\t");
		line_index += color_line(dbl, rt_line + line_index, lights[i].color);
		if (lights[i].u.l.visible == true)
		{
			dbl->value = lights[i].u.l.radius;
			dbltoa_buff_prec(*dbl);
			line_index += cpy_srcs(rt_line + line_index, "\t\t", dbl->buff, "\t\t");
			line_index += color_line(dbl, rt_line + line_index, lights[i].color);
		}
		ft_putendl_fd(rt_line, fd);
		++i;
	}
}
