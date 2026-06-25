#ifndef MATHRT_H
# define MATHRT_H
# include <math.h>
# include "miniRT.h"

# define EPSILON 1e-6F

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

/**
 * @brief Broadcasts scalar value to all four indexes of the t_vec4.
 * @param scalar Value to broadcast to vector[X, Y, Z, W].
 * @return (t_vec4){scalar, scalar, scalar, scalar}
 */
static inline t_vec4	bcast4(float scalar)
{
	return ((t_vec4){scalar, scalar, scalar, scalar});
}

/**
 * @brief Broadcasts scalar value to first three indexes of the t_vec4.
 * @param scalar Value to broadcast to vector[X, Y, Z].
 * @return (t_vec4){scalar, scalar, scalar, 1.0f}
 */
static inline t_vec4	bcast3(float scalar)
{
	return ((t_vec4){scalar, scalar, scalar, 1.0F});
}

//	clamp.c
float		clamp(float value, float min, float max);
int			intclamp(int value, int min, int max);
t_vec4		vec_clamp(t_vec4 value, float min, float max);

//	vec_arithmetic.c
t_vec4		vadd(t_vec4 a, t_vec4 b)__attribute__((const, hot));
t_vec4		vsub(t_vec4 a, t_vec4 b)__attribute__((const, hot));
t_vec4		vscale(t_vec4 v, float scalar)__attribute__((const, hot));

//	vec_geometry.c
t_vec4		vcross(t_vec4 a, t_vec4 b)__attribute__((const, hot));
t_vec4		vnorm(t_vec4 v)__attribute__((const, hot));
t_vec4		vproj(t_vec4 a, t_vec4 b)__attribute__((const, hot));
float		vdot(t_vec4 a, t_vec4 b)__attribute__((const, hot));
float		vlen(t_vec4 v)__attribute__((const, hot));

//	vec_transform.c
t_vec4		vrefl(t_vec4 v, t_vec4 n)__attribute__((const, hot));
t_vec4		vrotate(t_vec4 v, t_vec4 axis, float angle);

#endif
