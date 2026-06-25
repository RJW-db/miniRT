#include "scene.h"
#include "render.h"
#include "mathRT.h"

static bool		calc_hard_shadow(t_light_ctx *lx);
static void		init_light_ctx(t_light_ctx *lx, t_scene *sc, t_hit *hit, uint32_t i);
static float	calc_light_contrib(t_light_ctx *lx);

t_objs	*render_light(t_scene *sc, t_ray ray, float *hit_t, t_objs *closest_obj)
{
	uint32_t	i;
	float		t;

	i = 0;
	while (i < sc->l.l_arr_size)
	{
		if (sc->l.lights[i].u.l.visible == true
			|| sc->l.lights[i].u.l.intersect_lights == true)
		{
			if (ray_intersect_light(ray, &sc->l.lights[i], &t) && t < *hit_t)
			{
				closest_obj = sc->l.lights + i;
				*hit_t = t;
			}
		}
		++i;
	}
	return (closest_obj);
}

t_vec4	calc_lighting(t_scene *sc, t_hit *hit)
{
	t_light_ctx	lx;
	t_vec4		result;
	uint32_t	i;

	result = hit->color * sc->ambient.color * bcast3(sc->ambient.u.a.ratio);
	i = 0;
	while (i < sc->l.l_arr_size)
	{
		init_light_ctx(&lx, sc, hit, i);
		result += (hit->color * sc->l.lights[i].color
				* bcast3(calc_light_contrib(&lx)));
		++i;
	}
	return (vec_clamp(result, 0.0F, 1.0F));
}

static void	init_light_ctx(t_light_ctx *lx, t_scene *sc, t_hit *hit, uint32_t i)
{
	lx->sc = sc;
	lx->hit = hit;
	lx->light_i = i;
	lx->light_dir = vnorm(vsub(sc->l.lights[i].coords, hit->point));
	lx->distance = vlen(vsub(sc->l.lights[i].coords, hit->point));
}

static float	calc_light_contrib(t_light_ctx *lx)
{
	float	shadow;
	float	diff;

	if (lx->sc->soft_shadows == false || lx->sc->shadow_grsize < 2)
		shadow = calc_hard_shadow(lx);
	else
		shadow = calc_soft_shadow_circle(lx, (uint8_t)lx->sc->shadow_grsize);
	diff = clamp(vdot(lx->hit->normal, lx->light_dir), 0.0F, 1.0F);
	diff *= lx->sc->l.lights[lx->light_i].u.l.brightness;
	return (diff * shadow);
}

static bool	calc_hard_shadow(t_light_ctx *lx)
{
	t_ray		shadow_ray;
	uint32_t	i;
	float		t;

	if (vdot(lx->hit->normal, lx->light_dir) < 0.0F)
		return (0.0F);
	shadow_ray.origin = vadd(lx->hit->point,
			vscale(lx->light_dir, SHADOW_EPSILON));
	shadow_ray.vec = lx->light_dir;
	i = 0;
	while (i < lx->sc->o.o_arr_size)
	{
		if (ray_intersect_table(shadow_ray, &lx->sc->o.objs[i], &t)
			&& t > SHADOW_EPSILON && t < lx->distance)
			return (0.0F);
		++i;
	}
	return (1.0F);
}
