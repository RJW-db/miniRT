
#include "scene.h"
#include "render.h"
#include "mathRT.h"

// Static Functions
static bool		fill_hit_obj(t_scene *sc, t_ray ray, t_hit *hit);
static bool		fill_hit_light(t_scene *sc, t_ray ray, t_hit *hit);

uint8_t	ray_intersect_table(t_ray ray, t_objs *obj, float *t)
{
	static uint8_t	(*intersect_obj[NUM_OBJ_TYPES])(t_ray, t_objs *, float *) = {
		ray_intersect_plane,
		ray_intersect_sphere,
		ray_intersect_cylinder
	};

	return (intersect_obj[obj->type](ray, obj, t));
}

uint32_t	find_closest_object(t_scene *sc, t_ray ray, float *hit_t, uint8_t *hit_type)
{
	uint8_t		intersect_type;
	uint32_t	closest_obj;
	uint32_t	i;
	float		t;

	i = 0;
	closest_obj = 0;
	*hit_t = INFINITY;
	*hit_type = 0;
	while (i < sc->o.o_arr_size)
	{
		intersect_type = ray_intersect_table(ray, sc->o.objs + i, &t);
		if (intersect_type && t < *hit_t)
		{
			closest_obj = i;
			*hit_t = t;
			*hit_type = intersect_type;
		}
		++i;
	}
	return (closest_obj);
}

bool	find_closest_hit(t_scene *sc, t_ray ray, t_hit *hit)
{
	if (!fill_hit_obj(sc, ray, hit))
		return (fill_hit_light(sc, ray, hit));
	fill_hit_light(sc, ray, hit);
	if (hit->t == INFINITY || hit->t <= 0.0F)
		return (false);
	if (hit->obj->type == LIGHT)
		hit->color = hit->obj->u.l.obj_color;
	else
		hit->color = hit->obj->color;
	return (true);
}

t_vec4	trace_ray(t_scene *sc, t_ray ray)
{
	t_hit	hit;

	hit = (t_hit){0};
	hit.t = INFINITY;
	if (!find_closest_hit(sc, ray, &hit))
		return ((t_vec4){0.0F, 0.0F, 0.0F, 1.0F});
	if (hit.obj->type == LIGHT)
	{
		if (hit.obj->u.l.visible == false)
			return (hit.obj->color);
		return (hit.obj->u.l.obj_color);
	}
	return (calc_lighting(sc, &hit));
}

static bool	fill_hit_obj(t_scene *sc, t_ray ray, t_hit *hit)
{
	uint32_t	obj_i;

	obj_i = find_closest_object(sc, ray, &hit->t, &hit->type);
	if (hit->t == INFINITY || hit->t <= 0.0F)
		return (false);
	hit->obj = sc->o.objs + obj_i;
	hit->point = vadd(ray.origin, vscale(ray.vec, hit->t));
	hit->normal = calculate_normal(hit->obj, &ray, hit->t, hit->type);
	return (true);
}

static bool	fill_hit_light(t_scene *sc, t_ray ray, t_hit *hit)
{
	t_objs	*closest_obj;
	float	hit_t;

	closest_obj = hit->obj;
	hit_t = hit->t;
	closest_obj = render_light(sc, ray, &hit_t, closest_obj);
	if (hit_t >= hit->t || closest_obj == hit->obj)
		return (false);
	hit->obj = closest_obj;
	hit->t = hit_t;
	hit->type = 0;
	return (true);
}
