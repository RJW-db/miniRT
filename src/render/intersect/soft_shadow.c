#include "scene.h"
#include "render.h"
#include "mathRT.h"

static bool		is_occluded(t_light_ctx *lx, t_vec4 dir);
static float	lerp(float a, float b, float t);
static t_vec4	get_sample_dir(t_light_ctx *lx, t_vec4 tangent, t_vec4 bitangent, float angle);

float	calc_soft_shadow_circle(t_light_ctx *lx, uint8_t sample_count)
{
	t_vec4	tangent;
	t_vec4	bitangent;
	float	vis;
	float	angle;
	uint8_t	i;

	tangent = vnorm(vcross(lx->light_dir, lx->hit->normal));
	if (vlen(tangent) < 0.001F)
		tangent = (t_vec4){1.0F, 0.0F, 0.0F, 0.0F};
	bitangent = vnorm(vcross(lx->light_dir, tangent));
	vis = 0.0F;
	i = 0;
	while (i < sample_count)
	{
		angle = (2.0F * 3.14159F * i) / sample_count;
		if (!is_occluded(lx, get_sample_dir(lx, tangent, bitangent, angle)))
			vis += 1.0F;
		++i;
	}
	return (lerp(vis / (float)sample_count, 1.0F,
			clamp((lx->distance - lx->sc->l.lights[lx->light_i].u.l.radius)
				/ (lx->sc->l.lights[lx->light_i].u.l.radius * 0.1F),
				0.0F, 1.0F)));
}

static t_vec4	get_sample_dir(t_light_ctx *lx, t_vec4 tangent, t_vec4 bitangent, float angle)
{
	t_vec4	offset;
	float	radius;

	radius = lx->sc->l.lights[lx->light_i].u.l.radius;
	offset = vadd(vscale(tangent, cosf(angle) * radius),
			vscale(bitangent, sinf(angle) * radius));
	return (vnorm(vsub(vadd(lx->sc->l.lights[lx->light_i].coords, offset),
				lx->hit->point)));
}

static bool	is_occluded(t_light_ctx *lx, t_vec4 dir)
{
	t_ray		ray;
	uint16_t	j;
	float		t;

	ray.origin = vadd(lx->hit->point, vscale(dir, SHADOW_EPSILON));
	ray.vec = dir;
	j = 0;
	while (j < lx->sc->o.o_arr_size)
	{
		if (ray_intersect_table(ray, &lx->sc->o.objs[j], &t)
			&& t > SHADOW_EPSILON && t < lx->distance)
			return (true);
		++j;
	}
	return (false);
}

static float	lerp(float a, float b, float t)
{
	return (a + t * (b - a));
}
