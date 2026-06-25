#include <fcntl.h>
#include "scene.h"

//	Static Function
static t_dbltoa	setup_dbltoa_params(char *buff);

void	create_scene_rt_file(t_scene *sc, const char *filename)
{
	char		dbltoa_buff[MAX_DBL_BUFF];
	t_dbltoa	dbl;
	int			fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("create_scene_rt_file: Error opening fileeee");
		return ;
	}
	dbl = setup_dbltoa_params(dbltoa_buff);
	ambient_line(&sc->ambient, &dbl, fd);
	camera_line(&sc->camera, &dbl, fd);
	lights_line(sc->l.lights, sc->l.l_arr_size, &dbl, fd);
	objs_line(sc->o.objs, sc->o.o_arr_size, &dbl, fd);
	close(fd);
}

size_t	color_line(t_dbltoa *dbl, char *buf, t_vec4 color)
{
	size_t	line_idx;

	dbl->value = color[R] * 255;
	dbl->precision = 0;
	dbltoa_buff_prec(*dbl);
	line_idx = cpy_str0(buf, dbl->buff);
	buf[line_idx++] = ',';
	dbl->value = color[G] * 255;
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	buf[line_idx++] = ',';
	dbl->value = color[B] * 255;
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	return (line_idx);
}

size_t	coords_line(t_dbltoa *dbl, char *buf, t_vec4 coords)
{
	size_t	line_idx;

	dbl->value = coords[X];
	dbl->precision = 3;
	dbl->trim_trailing_zeros = true;
	dbltoa_buff_prec(*dbl);
	line_idx = cpy_str0(buf, dbl->buff);
	buf[line_idx++] = ',';
	dbl->value = coords[Y];
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	buf[line_idx++] = ',';
	dbl->value = coords[Z];
	dbltoa_buff_prec(*dbl);
	line_idx += cpy_str0(buf + line_idx, dbl->buff);
	line_idx += cpy_str0(buf + line_idx, "\t\t");
	return (line_idx);
}

static t_dbltoa	setup_dbltoa_params(char *buff)
{
	t_dbltoa	params;

	params.value = 0;
	params.buff = buff;
	params.buff_size = MAX_DBL_BUFF;
	params.precision = 1;
	params.trim_trailing_zeros = false;
	return (params);
}
