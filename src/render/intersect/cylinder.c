#include "scene.h"
#include "render.h"
#include "mathRT.h"

//	Static Function
static bool	solve_quadratic(const float a, const float b, const float c, t_cyl *cy);

uint8_t	ray_intersect_cylinder(t_ray ray, t_objs *obj, float *t)
{
	t_cyl	cy;
	uint8_t	hit_type;

	cy.ca = vnorm(obj->u.cylinder.orientation);
	cy.oc = vsub(ray.origin, obj->coords);
	cy.rd = vsub(ray.vec, vscale(cy.ca, vdot(ray.vec, cy.ca)));
	cy.oc_proj = vsub(cy.oc, vscale(cy.ca, vdot(cy.oc, cy.ca)));
	cy.half_height = obj->u.cylinder.height / 2.0F;
	cy.valid_t = -1.0F;
	if (!solve_quadratic(vdot(cy.rd, cy.rd), 2.0F * vdot(cy.rd, cy.oc_proj), \
		vdot(cy.oc_proj, cy.oc_proj) - obj->u.cylinder.radius * obj->u.cylinder.radius, &cy))
		return (CYL_NONE);
	if (cy.t[1] < 0.0F)
		return (CYL_NONE);
	if (cy.t[0] < 0.0F)
		cy.t[0] = cy.t[1];
	hit_type = CYL_NONE;
	hit_type = check_body_intersection(ray, obj, &cy);
	hit_type = check_cyl_caps(&cy, ray, obj, hit_type);
	if (cy.valid_t < 0.0F)
		return (CYL_NONE);
	*t = cy.valid_t;
	return (hit_type);
}

// Solves the quadratic equation: a*t^2 + b*t + c = 0.
// Returns true if real solutions exist and assigns *t0 and *t1 (ensuring *t0 <= *t1).
static bool	solve_quadratic(const float a, const float b, const float c, t_cyl *cy)
{
	const float	disc = b * b - 4.0F * a * c;
	float		sqrt_disc;
	float		tmp;

	if (disc < 0.0F)
		return (false);
	sqrt_disc = sqrtf(disc);
	cy->t[0] = (-b - sqrt_disc) / (2.0F * a);
	cy->t[1] = (-b + sqrt_disc) / (2.0F * a);
	if (cy->t[0] > cy->t[1])
	{
		tmp = cy->t[0];
		cy->t[0] = cy->t[1];
		cy->t[1] = tmp;
	}
	return (true);
}
