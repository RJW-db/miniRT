#include "scene.h"
#include "mathRT.h"

// Vector cross-product.
t_vec4	vcross(t_vec4 a, t_vec4 b)
{
	return ((t_vec4){
		a[Y] * b[Z] - a[Z] * b[Y],
		a[Z] * b[X] - a[X] * b[Z],
		a[X] * b[Y] - a[Y] * b[X]
	});
}

// Vector normalization.
t_vec4	vnorm(t_vec4 v)
{
	const t_vec4	len = bcast4(vlen(v));

	if (len[0] < EPSILON)
		return (v);
	return (v / len);
}

// Vector projection.
t_vec4	vproj(t_vec4 a, t_vec4 b)
{
	return (vscale(b, vdot(a, b) / vdot(b, b)));
}

// Vector length.
float	vlen(t_vec4 v)
{
	const t_vec4	vsquared = v * v;

	return (sqrtf(vsquared[X] + vsquared[Y] + vsquared[Z]));
}

// Vector dot-product.
float	vdot(t_vec4 a, t_vec4 b)
{
	const t_vec4	vmult = a * b;

	return (vmult[X] + vmult[Y] + vmult[Z]);
}
