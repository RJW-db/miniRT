#include <parsing.h>
#include <fcntl.h>

//	Static Functions
static bool	init_dynarr(t_value_check *sc);
static bool	shrink_dynarr(t_value_check *sc);
static void	free_dynarrs_on_failure(t_value_check *vc);

int16_t	parse_scene_file(const char *file, t_scene *sc)
{
	const int		fd = open(file, O_RDONLY);
	t_value_check	vc;

	ft_bzero(&vc, sizeof(t_value_check));
	if (fd < 0)
		return (errset(perr("parse_scene_file open", (int16_t)errno)));
	if (ft_strncmp(file + (ft_strlen(file) - 3), ".rt", 4) != 0)
		return (close(fd), errset(perr("parse_scene_file not .rt", ERRFEXT)));
	if (init_dynarr(&vc) == false)
		return (close(fd), errset(perr("init_dynarr", ENOMEM)));
	if (line_validation(fd, sc, &vc) != 0)
		return (free_dynarrs_on_failure(&vc), close(fd), errset(ERTRN));
	if (shrink_dynarr(&vc) == false) {
		free_dynarrs_on_failure(&vc);
		return (close(fd), errset(perr("shrink_dynarr", ENOMEM)));
	}
	sc->o.o_arr_size = vc.obj_dynarr.length;
	sc->l.l_arr_size = vc.light_dynarr.length;
	sc->o.objs = (t_objs *)dynarr_take_arr(&vc.obj_dynarr);
	sc->l.lights = (t_objs *)dynarr_take_arr(&vc.light_dynarr);
	close(fd);
	return (errset(ERTRN));
}

static bool	init_dynarr(t_value_check *vc)
{
	if (dynarr_create(&vc->obj_dynarr, 150, sizeof(t_objs)) == false)
		return (false);
	if (dynarr_create(&vc->light_dynarr, 10, sizeof(t_objs)) == false)
		return (false);
	return (true);
}

static bool	shrink_dynarr(t_value_check *vc)
{
	if (dynarr_shrink_to_fit(&vc->obj_dynarr) == false && errno == ENOMEM)
		return (false);
	if (vc->light_dynarr.length < vc->light_dynarr.capacity &&
	dynarr_shrink_to_fit(&vc->light_dynarr) == false && errno == ENOMEM)
		return (false);
	return (true);
}

static void	free_dynarrs_on_failure(t_value_check *vc)
{
	dynarr_free(&vc->obj_dynarr);
	dynarr_free(&vc->light_dynarr);
}