#include <mathRT.h>
#include <render.h>

//	Static functions
static t_vec4	calculate_normal(t_objs *obj, t_ray *ray, float t, uint8_t intersect_type);
static t_vec4	calculate_normal_cylinder(t_objs *obj, t_ray ray, float t, uint8_t intersect_type);

uint8_t	ray_intersect_table(t_ray ray, t_objs *obj, float *t)
{
	static uint8_t	(*intersect_obj[NUM_OBJ_TYPES])(t_ray, t_objs *, float *) = {
		ray_intersect_plane,
		ray_intersect_sphere,
		ray_intersect_cylinder
	};

	return (intersect_obj[obj->type](ray, obj, t));
}

uint32_t	find_closest_object(t_scene *sc, t_ray ray, float *closest_t, uint8_t *closest_intersect_type)
{
	uint32_t	closest_obj;
	uint32_t	i;
	float		t;
	uint8_t		intersect_type;

	closest_obj = 0;
	i = 0;
	*closest_t = INFINITY;
	*closest_intersect_type = 0;
	while (i < sc->o.o_arr_size)
	{
		intersect_type = ray_intersect_table(ray, sc->o.objs + i, &t);
		if (intersect_type && t < *closest_t)
		{
			closest_obj = i;
			*closest_t = t;
			*closest_intersect_type = intersect_type;
		}
		++i;
	}
	return (closest_obj);
}


t_vec4	trace_ray(t_scene *sc, t_ray ray)
{
	uint8_t	closest_intersect_type;
	t_objs	*closest_obj;
	t_vec4	pixel_color;
	float	closest_t;
	t_vec4	hit_point;

	pixel_color = (t_vec4){0.0F, 0.0F, 0.0F, 1.0F};
	closest_obj = sc->o.objs + find_closest_object(sc, ray, &closest_t, &closest_intersect_type);
	closest_obj = render_light(sc, ray, &closest_t, closest_obj);
	if (closest_t < INFINITY && closest_t > 0.0F)
	{
		pixel_color = closest_obj->color;
		if (closest_obj->type == LIGHT)
		{
			if (closest_obj->u.l.visible == false)
				return (pixel_color);
			return (closest_obj->u.l.obj_color);
		}
		hit_point = vadd(ray.origin, vscale(ray.vec, closest_t));
		return (calc_lighting(sc, hit_point, \
				calculate_normal(closest_obj, &ray, closest_t, \
				closest_intersect_type), pixel_color));
	}
	return (pixel_color);
}

static t_vec4	calculate_normal(t_objs *obj, t_ray *ray, float t, uint8_t intersect_type)
{
	t_vec4	normal;

	if (obj->type == PLANE)
	{
		if (vdot(obj->u.plane.orientation, ray->vec) > 0.0F)
		{
			return (vscale(obj->u.plane.orientation, -1.0F));
		}
		return (obj->u.plane.orientation);
	}
	normal = (t_vec4){0.0F, 0.0F, 0.0F, 1.0F};
	if (obj->type == SPHERE)
	{
		normal = vnorm(vsub(vadd(ray->origin, vscale(ray->vec, t)), obj->coords));
	}
	if (obj->type == CYLINDER)
	{
		normal = calculate_normal_cylinder(obj, *ray, t, intersect_type);
	}
	if (vdot(normal, ray->vec) > 0.0F)
		normal *= -1.0F;
	return (normal);
}

static t_vec4	calculate_normal_cylinder(t_objs *obj, t_ray ray, float t, uint8_t intersect_type)
{
	const t_vec4	hit_point = vadd(ray.origin, vscale(ray.vec, t));
	t_vec4			normal;

	if (intersect_type == CYL_BODY)
	{
		normal = vsub(hit_point, 
				vadd(obj->coords, 
				vscale(obj->u.cylinder.orientation, 
				vdot(vsub(hit_point, obj->coords), obj->u.cylinder.orientation))));
		return (vnorm(normal));
	}
	return (vnorm(obj->u.cylinder.orientation));
}
