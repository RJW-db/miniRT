#include "scene.h"
#include "render.h"
#include "mathRT.h"

// Static Functions
static t_vec4	cylinder_normal(t_objs *obj, t_ray ray, float t, uint8_t hit_type);

t_vec4	calculate_normal(t_objs *obj, t_ray *ray, float t, uint8_t intersect_type)
{
	t_vec4	normal;

	if (obj->type == PLANE)
	{
		if (vdot(obj->u.plane.orientation, ray->vec) > 0.0F)
			return (vscale(obj->u.plane.orientation, -1.0F));
		return (obj->u.plane.orientation);
	}
	normal = (t_vec4){0.0F, 0.0F, 0.0F, 1.0F};
	if (obj->type == SPHERE)
		normal = vnorm(vsub(vadd(ray->origin, vscale(ray->vec, t)),
					obj->coords));
	if (obj->type == CYLINDER)
		normal = cylinder_normal(obj, *ray, t, intersect_type);
	if (vdot(normal, ray->vec) > 0.0F)
		normal = vscale(normal, -1.0F);
	return (normal);
}

static t_vec4	cylinder_normal(t_objs *obj, t_ray ray, float t, uint8_t hit_type)
{
	t_vec4	hit_point;
	t_vec4	axis_point;
	t_vec4	offset;

	hit_point = vadd(ray.origin, vscale(ray.vec, t));
	if (hit_type != CYL_BODY)
		return (vnorm(obj->u.cylinder.orientation));
	axis_point = vadd(obj->coords,
			vscale(obj->u.cylinder.orientation,
				vdot(vsub(hit_point, obj->coords),
					obj->u.cylinder.orientation)));
	offset = vsub(hit_point, axis_point);
	return (vnorm(offset));
}
