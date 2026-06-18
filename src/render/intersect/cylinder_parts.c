#include "scene.h"
#include "render.h"
#include "mathRT.h"

//	Static Functions
static \
inline t_vec4	ray_at(t_ray ray, float t);
static uint8_t	get_cap_intersection(t_cyl_cap *cycap, t_ray ray, t_vec4 ca, uint8_t cap_type);
static bool		intersect_cylinder_caps(t_ray ray, t_ray plane, float *t);

// Checks if one of the body intersections is valid (i.e. within the cylinder's finite height).
uint8_t	check_body_intersection(t_ray ray, t_objs *obj, t_cyl *cy)
{
	const t_vec4	hit0 = ray_at(ray, cy->t[0]);
	const t_vec4	hit1 = ray_at(ray, cy->t[1]);
	const float		y0 = vdot(vsub(hit0, obj->coords), cy->ca);
	const float		y1 = vdot(vsub(hit1, obj->coords), cy->ca);

	if (y0 >= -cy->half_height && y0 <= cy->half_height)
	{
		cy->valid_t = cy->t[0];
		return (CYL_BODY);
	}
	else if (y1 >= -cy->half_height && y1 <= cy->half_height)
	{
		cy->valid_t = cy->t[1];
		return (CYL_BODY);
	}
	return (CYL_NONE);
}

uint8_t	check_cyl_caps(t_cyl *cy, t_ray ray, t_objs *obj, uint8_t hit_type)
{
	t_cyl_cap	cycap;
	uint8_t		cap_hit;

	cycap.top_cap = vadd(obj->coords, vscale(cy->ca, cy->half_height));
	cycap.bottom_cap = vsub(obj->coords, vscale(cy->ca, cy->half_height));
	cycap.cap_radius = obj->u.cylinder.radius;
	cap_hit = get_cap_intersection(&cycap, ray, cy->ca, CYL_TOP);
	if (cap_hit != CYL_NONE && (cy->valid_t < 0.0F || cycap.t_cap < cy->valid_t))
	{
		cy->valid_t = cycap.t_cap;
		hit_type = cap_hit;
	}
	cap_hit = get_cap_intersection(&cycap, ray, cy->ca, CYL_BOTTOM);
	if (cap_hit != CYL_NONE && (cy->valid_t < 0.0F || cycap.t_cap < cy->valid_t))
	{
		cy->valid_t = cycap.t_cap;
		hit_type = cap_hit;
	}
	return (hit_type);
}

// Returns the point along the ray at parameter t.
static inline t_vec4	ray_at(t_ray ray, float t)
{
	return (vadd(ray.origin, vscale(ray.vec, t)));
}

// Checks the intersection with one cylinder cap and updates t_hit if valid.
static uint8_t	get_cap_intersection(t_cyl_cap *cycap, t_ray ray, t_vec4 ca, uint8_t cap_type)
{
	float	t_cap;
	t_vec4	center;
	t_vec4	p;
	t_vec4	d;

	if (cap_type == CYL_TOP)
		center = cycap->top_cap;
	else
		center = cycap->bottom_cap;
	if (intersect_cylinder_caps(ray, (t_ray){center, ca}, &t_cap))
	{
		p = ray_at(ray, t_cap);
		d = vsub(p, center);
		if (vdot(d, d) <= cycap->cap_radius * cycap->cap_radius)
		{
			cycap->t_cap = t_cap;
			return (cap_type);
		}
	}
	return (CYL_NONE);
}

// Computes the intersection of a ray and a plane (for caps).
static bool	intersect_cylinder_caps(t_ray ray, t_ray plane, float *t)
{
	const float	denom = vdot(plane.vec, ray.vec);
	t_vec4		origin_to_plane;

	if (fabs(denom) > 1e-6F)
	{
		origin_to_plane = vsub(plane.origin, ray.origin);
		*t = vdot(origin_to_plane, plane.vec) / denom;
		return (*t >= 0.0F);
	}
	return (false);
}
