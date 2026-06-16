#include <fcntl.h>
#include <dbltoa.h>
#include <scene.h>

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
	lights_line(sc->lights, sc->l_arr_size, &dbl, fd);
	objs_line(sc->objs, sc->o_arr_size, &dbl, fd);
	close(fd);
}

size_t	color_line(t_dbltoa *dbl, char *rt_line, t_vec4 color)
{
	size_t	line_index;

	dbl->value = color[R] * 255;
	dbl->precision = 0;
	dbltoa_buff_prec(*dbl);
	line_index = cpy_str0(rt_line, dbl->buff);
	rt_line[line_index++] = ',';
	dbl->value = color[G] * 255;
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	rt_line[line_index++] = ',';
	dbl->value = color[B] * 255;
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	return (line_index);
}

size_t	coords_line(t_dbltoa *dbl, char *rt_line, t_vec4 coords)
{
	size_t	line_index;

	dbl->value = coords[X];
	dbl->precision = 3;
	dbl->trim_trailing_zeros = true;
	dbltoa_buff_prec(*dbl);
	line_index = cpy_str0(rt_line, dbl->buff);
	rt_line[line_index++] = ',';
	dbl->value = coords[Y];
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	rt_line[line_index++] = ',';
	dbl->value = coords[Z];
	dbltoa_buff_prec(*dbl);
	line_index += cpy_str0(rt_line + line_index, dbl->buff);
	line_index += cpy_str0(rt_line + line_index, "\t\t");
	return (line_index);
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
