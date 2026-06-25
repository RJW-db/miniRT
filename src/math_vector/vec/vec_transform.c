#include "scene.h"
#include "mathRT.h"

// Vector reflection.
t_vec4	vrefl(t_vec4 v, t_vec4 n)
{
	return (vsub(v, vscale(n, 2.0F * vdot(v, n))));
}

// Vector rotate around an axis.
t_vec4	vrotate(t_vec4 v, t_vec4 axis, float angle)
{
	const float		cos_angle = cosf(angle);
	const float		sin_angle = sinf(angle);

	return ((t_vec4){
		v[X] * (cos_angle + axis[X] * axis[X] * (1 - cos_angle)) +
		v[Y] * (axis[X] * axis[Y] * (1 - cos_angle) - axis[Z] * sin_angle) +
		v[Z] * (axis[X] * axis[Z] * (1 - cos_angle) + axis[Y] * sin_angle),
		v[X] * (axis[Y] * axis[X] * (1 - cos_angle) + axis[Z] * sin_angle) +
		v[Y] * (cos_angle + axis[Y] * axis[Y] * (1 - cos_angle)) +
		v[Z] * (axis[Y] * axis[Z] * (1 - cos_angle) - axis[X] * sin_angle),
		v[X] * (axis[Z] * axis[X] * (1 - cos_angle) - axis[Y] * sin_angle) +
		v[Y] * (axis[Z] * axis[Y] * (1 - cos_angle) + axis[X] * sin_angle) +
		v[Z] * (cos_angle + axis[Z] * axis[Z] * (1 - cos_angle)),
	});
}
