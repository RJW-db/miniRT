#include "mathRT.h"

// Vector addition.
t_vec4	vadd(t_vec4 a, t_vec4 b)
{
	return (a + b);
}

// Vector substraction.
t_vec4	vsub(t_vec4 a, t_vec4 b)
{
	return (a - b);
}

// Vector scaling.
t_vec4	vscale(t_vec4 v, float scalar)
{
	return (v * bcast4(scalar));
}
