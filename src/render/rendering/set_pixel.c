#include "RTmlx.h"
#include "render.h"

void	scaled_res_set_pixel(t_window *w, uint16_t x, uint16_t y, t_vec4 color)
{
	uint8_t				*pixels;
	t_axis2				i;
	t_axis2				pixel;

	color *= 255.0F;
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
				*(pixels++) = (uint8_t)color[R];
				*(pixels++) = (uint8_t)color[G];
				*(pixels++) = (uint8_t)color[B];
				*(pixels++) = (uint8_t)color[A];
			}
			++i.x;
		}
		++i.y;
	}
}

void	set_pixel_multi(t_thread *th, uint16_t res_ratio, t_axis2 inp, t_vec4 color)
{
	uint8_t	*pixels;
	t_axis2	i;
	t_axis2	pix;

	color *= 255.0F;
	i.y = 0;
	while (i.y < res_ratio)
	{
		i.x = 0;
		pix.y = (uint16_t)(inp.y * res_ratio + i.y);
		while (i.x < res_ratio)
		{
			pix.x = (uint16_t)(inp.x * res_ratio + i.x);
			if (pix.x < th->img->width && pix.y < th->img->height)
			{
				pixels = th->pixels + (pix.y * th->img->width + pix.x) * 4;
				*(pixels++) = (uint8_t)color[R];
				*(pixels++) = (uint8_t)color[G];
				*(pixels++) = (uint8_t)color[B];
				*(pixels++) = (uint8_t)color[A];
			}
			++i.x;
		}
		++i.y;
	}
}
