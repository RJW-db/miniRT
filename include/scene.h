#ifndef SCENE_H
# define SCENE_H

# include <dbltoa.h>
# include "forward_declarations.h"
# include <miniRT.h>

# define RT_MAX_LINE_LEN 150
//	3 objects, Plane Sphere Cylinder
# define NUM_OBJ_TYPES 3

enum e_axis
{
	X,
	Y,
	Z,
	W
};

enum e_rgba
{
	R,
	G,
	B,
	A
};

typedef struct s_axis2_16
{
	uint16_t		x;
	uint16_t		y;
}	t_axis2;

typedef struct s_ray
{
	t_vec4		origin;
	t_vec4		vec;
}	t_ray;

typedef struct s_light
{
	t_vec4		obj_color;
	float		brightness;
	float		radius;
	bool		intersect_lights;
	bool		visible;
}	t_light;

typedef struct s_amblight
{
	float		ratio;
}	t_amblight;

typedef enum e_obj_types
{
	PLANE,
	SPHERE,
	CYLINDER,
	LIGHT,
	AMBIENT
}	t_obj_type;

typedef struct	s_plane
{
	t_vec4	orientation;
}	t_plane;

typedef struct	s_sphere
{
	float	radius;
	float	diameter;
}	t_sphere;

typedef struct	s_cylinder
{
	t_vec4	orientation;
	float	radius;
	float	diameter;
	float	height;
}	t_cylinder;

typedef struct	s_camera
{
	t_vec4	orientation;
	float	fov;
	float	zvp_dist;
	float	cam_m_speed;
	float	cam_r_speed;
	float	cam_fov_speed;
}	t_camera;

typedef struct	s_objs
{
	t_obj_type		type;
	union u_union
	{
		t_camera	c;
		t_amblight	a;
		t_light		l;

		t_plane		plane;
		t_sphere	sphere;
		t_cylinder	cylinder;
	}	u;
	t_vec4			coords;
	t_vec4			color;
}	t_objs;

struct	s_scene
{
	bool		render;
	bool		render_ongoing;
	t_objs		camera;
	t_objs		ambient;
	struct s_objs_arr
	{
		t_objs		*objs;
		size_t		o_arr_size;	
	}	o;
	struct s_lights_arr
	{
		t_objs		*lights;
		size_t		l_arr_size;
	}	l;
	t_objs		*selected_obj;
	ssize_t		sel_obj_index;
	bool		soft_shadows;
	uint16_t	shadow_grsize;
};

void	create_scene_rt_file(t_scene *sc, const char *filename);
size_t	color_line(t_dbltoa *dbl, char *rt_line, t_vec4 color);
size_t	coords_line(t_dbltoa *dbl, char *rt_line, t_vec4 coords);

void	ambient_line(t_objs *ambient, t_dbltoa *dbl, int fd);
void	camera_line(t_objs *camera, t_dbltoa *dbl, int fd);
void	lights_line(t_objs *lights, size_t amount, t_dbltoa *dbl, int fd);

void	objs_line(t_objs *objs, size_t amount, t_dbltoa *dbl, int fd);

void	print_obj_info(t_scene *sc);
void	geometric_primitives(t_objs *obj);
#endif