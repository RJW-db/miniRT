#include "RTmlx.h"
#include "render.h"

//	Static Function
static inline uint32_t	pack_rgba(t_vec4 color);

void	scaled_res_set_pixel(t_window *w, uint16_t x, uint16_t y, t_vec4 color)
{
	uint8_t		*pixels;
	t_axis2		i;
	t_axis2		pixel;
	uint32_t	packed;

	color *= 255.0F;
	packed = pack_rgba(color);
	i.y = 0;
	while (i.y < w->res_ratio)
	{
		i.x = 0;
		pixel.y = (uint16_t)(y * w->res_ratio + i.y);
		while (i.x < w->res_ratio)
		{
			pixel.x = (uint16_t)(x * w->res_ratio + i.x);
			if (pixel.x < w->mlx->width && pixel.y < w->mlx->height)
			{
				pixels = w->pixels + (pixel.y * w->mlx->width + pixel.x) * 4;
				*(uint32_t *)pixels = packed;
			}
			++i.x;
		}
		++i.y;
	}
}

void	set_pixel_multi(t_thread *th, uint16_t res_ratio, t_axis2 inp, t_vec4 color)
{
	uint8_t		*pixels;
	t_axis2		i;
	t_axis2		pixel;
	uint32_t	packed;

	color *= 255.0F;
	packed = pack_rgba(color);
	i.y = 0;
	while (i.y < res_ratio)
	{
		i.x = 0;
		pixel.y = (uint16_t)(inp.y * res_ratio + i.y);
		while (i.x < res_ratio)
		{
			pixel.x = (uint16_t)(inp.x * res_ratio + i.x);
			if (pixel.x < th->img->width && pixel.y < th->img->height)
			{
				pixels = th->pixels + (pixel.y * th->img->width + pixel.x) * 4;
				*(uint32_t *)pixels = packed;
			}
			++i.x;
		}
		++i.y;
	}
}

static inline uint32_t	pack_rgba(t_vec4 color)
{
	return (
		((uint32_t)(uint8_t)color[R]) |
		((uint32_t)(uint8_t)color[G] << 8) |
		((uint32_t)(uint8_t)color[B] << 16) |
		((uint32_t)(uint8_t)color[A] << 24)
	);
}
