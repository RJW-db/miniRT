#include <limits.h>
#include "scene.h"
#include "RTmlx.h"
#include "render.h"
#include "mathRT.h"
#include "threadsRT.h"
#include "setup_clean.h"

//	Static Functions
static int	powi(int base, int pow);
static char	*itoa_simple(int num, char *buffer);
static char	*ftoa_simple(float num, char *buffer, uint8_t prec);
static void	fill_print_buffer(t_rt *rt, float avg_fps, char *buffer);

void	print_performance_stats(t_rt *rt, float fps)
{
	static float	fps_average = 0.0F;
	static uint8_t	counter = 0;
	static double	last_print_time = 0.0;
	const double	cur_time = mlx_get_time();
	char			buffer[PRT_BUFFER_SIZE];

	fps_average += fps;
	++counter;
	if (rt->win->res_ratio == rt->win->res_r_start \
		&& cur_time < last_print_time + 0.3)
		return ;
	last_print_time = cur_time;
	if (rt->win->prt_perf_stats == true)
	{
		fill_print_buffer(rt, fps_average / (float)counter, buffer);
		pthread_mutex_lock(rt->mtx + MTX_PRINT);
		ft_putstr_fd("\r\33[2K\033[94m", STDOUT_FILENO);
		if (rt->win->res_ratio == RES_R_FULL)
			ft_putstr_fd("\033[92m", STDOUT_FILENO);
		ft_putstr_fd(buffer, STDOUT_FILENO);
		ft_putstr_fd("\033[0m", STDOUT_FILENO);
		pthread_mutex_unlock(rt->mtx + MTX_PRINT);
	}
	counter = 0;
	fps_average = 0;
}

static void	fill_print_buffer(t_rt *rt, float avg_fps, char *buffer)
{
	if ((int)avg_fps > INT_MAX || avg_fps < 0.0F)
		return ;
	ft_bzero(buffer, PRT_BUFFER_SIZE);
	ft_strlcpy(buffer, " ResStep ", 10);
	buffer += 9;
	buffer = itoa_simple(rt->win->res_ratio, buffer);
	ft_strlcpy(buffer, " -> ", 5);
	buffer += 4;
	buffer = ftoa_simple(avg_fps, buffer, 1);
	ft_strlcpy(buffer, " fps(avg)", 10);
}

// Simple itoa, If buffer is too small: overflow (Segfault).
static char	*itoa_simple(int num, char *buffer)
{
	size_t		len;
	size_t		i;
	uint32_t	n;

	if (num == 0)
		return (buffer[0] = '0', buffer[1] = '\0', buffer + 1);
	i = 0;
	if (num < 0)
	{
		buffer[i++] = '-';
		n = (uint32_t)(-(int64_t)num);
	}
	else
		n = (uint32_t)num;
	len = 1;
	while (n / (uint32_t)powi(10, (int)len) != 0)
		++len;
	i += len;
	len = i;
	while (i > 0)
	{
		buffer[--i] = (char)('0' + n % 10);
		n /= 10;
	}
	return (buffer + len);
}

static int	powi(int base, int pow)
{
	const int	start_base = base;

	while (pow-- > 1)
		base *= start_base;
	return (base);
}

// Simple ftoa, If buffer size is too small: overflow (Segfault).
// Min precicion = 1, max = 255.
static char	*ftoa_simple(float num, char *buffer, uint8_t prec)
{
	float	fraction;
	int		int_part;
	int		digit;
	size_t	i;

	if (!buffer || prec < 1)
		return (buffer);
	int_part = (int)num;
	fraction = fabsf(num - (float)int_part);
	buffer = itoa_simple(int_part, buffer);
	*(buffer++) = '.';
	i = 0;
	while (i < prec)
	{
		fraction *= 10;
		digit = (int)fraction;
		buffer[i++] = (char)('0' + digit);
		fraction -= (float)digit;
	}
	return (buffer[i] = '\0', buffer + i);
}
