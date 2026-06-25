#include "mathRT.h"

// Clamp a value between min and max
float	clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

// Clamp a value between min and max
int	intclamp(int value, int min, int max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

// Clamp a vector between min and max
t_vec4	vec_clamp(t_vec4 value, float min, float max)
{
	return ((t_vec4){
		clamp(value[0], min, max),
		clamp(value[1], min, max),
		clamp(value[2], min, max),
		clamp(value[3], min, max)
	});
}
