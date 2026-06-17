#include <scene.h>
#include <render.h>
#include <mathRT.h>

//	Static Functions
static bool		calc_hard_shadow(t_scene *sc, t_ray ray, t_vec4 light_dir, uint32_t light);
static float	calc_soft_shadow_circle(t_scene *sc, t_ray ray, t_vec4 light_pos, uint32_t light, uint8_t sample_count);

t_objs	*render_light(t_scene *sc, t_ray ray, float *closest_t, t_objs *closest_obj)
{
	uint32_t	i;
	float		t;

	i = 0;
	while (i < sc->l.l_arr_size)
	{
		if (sc->l.lights[i].u.l.visible == true || sc->l.lights[i].u.l.intersect_lights == true)
		{
			if (ray_intersect_light(ray, &sc->l.lights[i], &t) && t < *closest_t)
			{
				closest_obj = sc->l.lights + i;
				*closest_t = t;
			}
		}
		++i;
	}
	return (closest_obj);
}

t_vec4	calc_lighting(t_scene *sc, t_vec4 point, t_vec4 normal, t_vec4 obj_color)
{
	t_vec4			light_dir;
	t_vec4			result;
	float			shadow;
	float			diff;
	uint32_t		i;

	result = obj_color * sc->ambient.color * bcast3(sc->ambient.u.a.ratio);
	i = 0;
	while (i < sc->l.l_arr_size)
	{
		light_dir = vnorm(vsub(sc->l.lights[i].coords, point));
		if (sc->soft_shadows == false || sc->shadow_grsize < 2)
		{
			if (calc_hard_shadow(sc, (t_ray){point, normal}, light_dir, i))
				shadow = 0.0F;
			else
				shadow = 1.0F;
		}
		else
			shadow = calc_soft_shadow_circle(sc, (t_ray){point, normal}, sc->l.lights[i].coords, i, (uint8_t)sc->shadow_grsize);
		diff = clamp(vdot(normal, light_dir), 0.0F, 1.0F) * sc->l.lights[i].u.l.brightness;
		result += (obj_color * sc->l.lights[i].color * bcast3(diff * shadow));
		++i;
	}
	return (vec_clamp(result, 0.0F, 1.0F));
}

static bool	calc_hard_shadow(t_scene *sc, t_ray ray, t_vec4 light_dir, uint32_t light)
{
	float		distance;
	t_vec4		origin;
	uint32_t	i;
	float		t;

	i = 0;
	t = 0.0F;
	origin = vadd(ray.origin, vscale(light_dir, SHADOW_EPSILON));
	distance = vlen(vsub(sc->l.lights[light].coords, ray.origin));
	if (vdot(ray.vec, light_dir) < 0.0F)
		return (true);
	while (i < sc->o.o_arr_size)
	{
		if (ray_intersect_table((t_ray){origin, light_dir}, &sc->o.objs[i], &t)
			&& t > SHADOW_EPSILON && t < distance)
			return (true);
		++i;
	}
	return (false);
}

//--------------- Soft shadows -----------------
// Work in progress.


// Linear interpolation helper: lerp(a, b, t) = a + t*(b - a)
static float	lerp(float a, float b, float t)
{
	return (a + t * (b - a));
}

bool	is_occluded(t_scene *sc, t_vec4 origin, t_vec4 dir, float max_dist)
{
	const t_ray	ray = {vadd(origin, vscale(dir, SHADOW_EPSILON)), dir};
	float		t;
	uint16_t	j;

	j = 0;
	while (j < sc->o.o_arr_size)
	{
		if (ray_intersect_table(ray, &sc->o.objs[j], &t)
			&& t > SHADOW_EPSILON && t < max_dist)
			return true;
		++j;
	}
	return false;
}

static float	calc_soft_shadow_circle(t_scene *sc, t_ray ray, t_vec4 light_pos, uint32_t light, uint8_t sample_count)
{
	t_vec4		light_dir;
	t_vec4		tangent, bitangent, sample_dir;
	float		distance, angle, vis;
	uint8_t		i;
	const float	radius = sc->l.lights[light].u.l.radius;

	light_dir = vnorm(vsub(light_pos, ray.origin));
	tangent = vnorm(vcross(light_dir, ray.vec));
	if (vlen(tangent) < 0.001F)
		tangent = (t_vec4){1.0F, 0.0F, 0.0F, 0.0F};
	bitangent = vnorm(vcross(light_dir, tangent));

	distance = vlen(vsub(light_pos, ray.origin));
	vis = 0.0F;
	i = 0;

	while (i < sample_count)
	{
		angle = (2.0F * 3.14159F * i) / sample_count;
		sample_dir = vadd(
			vscale(tangent, cosf(angle) * radius),
			vscale(bitangent, sinf(angle) * radius)
		);
		sample_dir = vnorm(vsub(vadd(light_pos, sample_dir), ray.origin));

		if (!is_occluded(sc, ray.origin, sample_dir, distance))
			vis += 1.0F;
		++i;
	}
	float softness = clamp((distance - radius) / (radius * 0.1F), 0.0F, 1.0F);
	return lerp(vis / (float)sample_count, 1.0F, softness);
}
