#include "scene.h"
#include "render.h"
#include "mathRT.h"

// Static Functions
static void		init_shadow_basis(t_light_ctx *lx, t_vec4 *tangent, t_vec4 *bitangent);
static bool		is_occluded(t_light_ctx *lx, t_vec4 dir);
static t_vec4	get_sample_dir(t_light_ctx *lx, t_vec4 tangent, t_vec4 bitangent, float angle);

float	calc_soft_shadow_circle(t_light_ctx *lx, uint8_t sample_count)
{
	t_vec4	tangent;
	t_vec4	bitangent;
	float	vis;
	uint8_t	i;
	t_vec4	sample_dir;

	if (vdot(lx->hit->normal, lx->light_dir) < 0.0F)
		return (0.0F);
	init_shadow_basis(lx, &tangent, &bitangent);
	vis = 0.0F;
	i = 0;
	while (i < sample_count)
	{
		sample_dir = get_sample_dir(lx, tangent, bitangent, (2.0F * 3.14159F * i) / sample_count);
		if (!is_occluded(lx, sample_dir))
			vis += 1.0F;
		++i;
	}
	return (vis / (float)sample_count);
}

static void	init_shadow_basis(t_light_ctx *lx, t_vec4 *tangent, t_vec4 *bitangent)
{
	*tangent = vnorm(vcross(lx->light_dir, lx->hit->normal));
	if (vlen(*tangent) < 0.001F)
		*tangent = (t_vec4){1.0F, 0.0F, 0.0F, 0.0F};
	*bitangent = vnorm(vcross(lx->light_dir, *tangent));
}

static t_vec4	get_sample_dir(t_light_ctx *lx, t_vec4 tangent, t_vec4 bitangent, float angle)
{
	t_vec4	offset;
	t_vec4	t_offset;
	t_vec4	b_offset;
	float	radius;

	radius = lx->sc->l.lights[lx->light_i].u.l.radius;
	t_offset = vscale(tangent, cosf(angle) * radius);
	b_offset = vscale(bitangent, sinf(angle) * radius);
	offset = vadd(t_offset, b_offset);
	return (vadd(lx->sc->l.lights[lx->light_i].coords, offset));
}

static bool	is_occluded(t_light_ctx *lx, t_vec4 sample_pos)
{
	t_ray		ray;
	uint16_t	j;
	float		t;
	t_vec4		to_light;
	float		max_dist;

	to_light = vsub(sample_pos, lx->hit->point);
	max_dist = vlen(to_light);
	if (max_dist <= SHADOW_EPSILON)
		return (false);
	ray.vec = vscale(to_light, 1.0F / max_dist);
	ray.origin = vadd(lx->hit->point, vscale(lx->hit->normal, SHADOW_EPSILON));
	j = 0;
	while (j < lx->sc->o.o_arr_size)
	{
		if (ray_intersect_table(ray, &lx->sc->o.objs[j], &t) &&
			t > SHADOW_EPSILON && t < max_dist - SHADOW_EPSILON)
		{
			return (true);
		}
		++j;
	}
	return (false);
}
