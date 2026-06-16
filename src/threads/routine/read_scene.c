#include <parsing.h>
#include <RTmlx.h>

int16_t	init_read_scene(t_scene *src, t_scene *dest)
{
	ft_memcpy(dest, src, sizeof(t_scene));
	dest->o.objs = NULL;
	dest->l.lights = NULL;
	dest->o.objs = (t_objs *)malloc(src->o.o_arr_size * sizeof(t_objs));
	if (dest->o.objs == NULL)
		return (errset(errno));
	dest->l.lights = (t_objs *)malloc(src->l.l_arr_size * sizeof(t_objs));
	if (dest->l.lights == NULL)
		return (errset(errno));
	ft_memcpy(dest->o.objs, src->o.objs, sizeof(t_objs) * src->o.o_arr_size);
	ft_memcpy(dest->l.lights, src->l.lights, sizeof(t_objs) * src->l.l_arr_size);
	return (EXIT_SUCCESS);
}

int16_t	cpy_scene(t_scene *src, t_scene *dest)
{
	t_objs	*dest_obj;
	t_objs	*dest_lights;

	dest_obj = dest->o.objs;
	dest_lights = dest->l.lights;
	ft_memcpy(dest, src, sizeof(t_scene));
	dest->o.objs = dest_obj;
	dest->l.lights = dest_lights;
	ft_memcpy(dest->o.objs, src->o.objs, sizeof(t_objs) * src->o.o_arr_size);
	ft_memcpy(dest->l.lights, src->l.lights, sizeof(t_objs) * src->l.l_arr_size);
	return (EXIT_SUCCESS);
}
