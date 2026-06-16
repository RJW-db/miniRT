#include <scene.h>
#include <mathRT.h>

uint8_t	ray_intersect_plane(t_ray ray, t_objs *obj, float *t)
{
	const float	denominator = vdot(ray.vec, obj->u.plane.orientation);
	t_vec4		to_center;

	if (fabs(denominator) > EPSILON)
	{
		to_center = vsub(obj->coords, ray.origin);
		*t = vdot(to_center, obj->u.plane.orientation) / denominator;
		return (*t >= 0.0F);
	}
	return (false);
}

uint8_t	ray_intersect_sphere(t_ray ray, t_objs *obj, float *t)
{
	const t_vec4	to_center = vsub(ray.origin, obj->coords);
	const t_vec4	abc = {
		vdot(ray.vec, ray.vec),
		2.0F * vdot(to_center, ray.vec),
		vdot(to_center, to_center) - obj->u.sphere.radius * obj->u.sphere.radius
	};
	const float		discriminant = abc[1] * abc[1] - 4.0F * abc[0] * abc[2];
	float			sqrt_discriminant;

	if (discriminant < 0.0F)
		return (false);
	sqrt_discriminant = sqrtf(discriminant);
	*t = (-abc[1] - sqrt_discriminant) / (2.0F * abc[0]);
	if (*t < 0.0F)
		*t = (-abc[1] + sqrt_discriminant) / (2.0F * abc[0]);
	return (*t >= 0.0F);
}

uint8_t	ray_intersect_light(t_ray ray, t_objs *obj, float *t)
{
	const t_vec4	to_center = vsub(ray.origin, obj->coords);
	const t_vec4	abc = {
		vdot(ray.vec, ray.vec),
		2.0F * vdot(to_center, ray.vec),
		vdot(to_center, to_center) - obj->u.l.radius * obj->u.l.radius
	};
	const float		discriminant = abc[1] * abc[1] - 4.0F * abc[0] * abc[2];
	float			sqrt_discriminant;

	if (discriminant < 0.0F)
		return (false);
	sqrt_discriminant = sqrtf(discriminant);
	*t = (-abc[1] - sqrt_discriminant) / (2.0F * abc[0]);
	if (*t < 0.0F)
		*t = (-abc[1] + sqrt_discriminant) / (2.0F * abc[0]);
	return (*t >= 0.0F);
}
