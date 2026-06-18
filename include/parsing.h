#ifndef PARSING_H
# define PARSING_H
# include "libftx.h"
# include "dynarr.h"
# include "scene.h"
# include "RTerror.h"

typedef struct s_parsing_value_check
{
	uint16_t	cam_amount;
	uint16_t	amb_amount;
	uint16_t	obj_amount;
	t_dynarr	obj_dynarr;
	t_dynarr	light_dynarr;
}	t_value_check;

static inline int	ft_isnum(const char *s)
{
	return (ft_isdigit(*s) || (*s == '-' && ft_isdigit(s[1])));
}

// parsing.c
int16_t		parse_scene_file(const char *file, t_scene *sc);

//	line_validation
int16_t		line_validation(const int fd, t_scene *sc, t_value_check *vc);
bool		check_line(char *line, const char prefix, uint8_t nbr_of_groups);

//	initialize_input_element
bool		input_type_parse(t_scene *sc, t_value_check *vc, char *line);
bool		init_primitives(t_value_check *vc, char *line);

// parse_utils.c
float		rt_atof(const char *str);
char		*nxtv(char *str);
char		*nxtvp(char **str);


bool		validate_orientation(t_vec4 *or, char **line);
bool		validate_and_normalize_color(t_vec4 *color, char **line);

// trash
void		_print_parsing(t_scene *sc);
#endif
