#ifndef THREADSRT_H
# define THREADSRT_H

# include <stdbool.h>
# include <stdint.h>
# include <MLX42/MLX42.h>
# include "forward_declarations.h"

# define PRT_BUFFER_SIZE 64

typedef float	t_vec4 __attribute__ ((vector_size(16)));

enum e_mtx
{
	MTX_PRINT,
	MTX_SYNC,
	MTX_RESYNC,
	MTX_DONE_RENDERING,
	MTX_QUIT_ROUTINE,
	MTX_STOPPED_THREADS,
	MTX_RENDER,
	MTX_SWITCH_BOOL,
	MTX_AMOUNT
};

typedef struct	s_thread
{
	t_rt			*rt;
	t_scene			*scene;
	t_window		*win;
	mlx_image_t		*img;
	uint8_t			*pixels;
	uint8_t			*pixels_mlx;
	uint8_t			*pixels_own;
	pthread_t		thread;
}	t_thread;


bool	res_setscale(t_window *win, const float scale);
void	print_performance_stats(t_rt *rt, float fps);

bool	initialize_mutexes(t_rt *rt);
bool	initialize_conditions(t_rt *rt);
bool	launch_pthreads(t_rt *rt);
void	destroy_threads(t_rt *rt);
bool	destroy_conditions(t_rt *rt);
void	destroy_mutexes(t_rt *rt, size_t amount);

bool	img_multithreaded(t_rt *rt);
void	*thread_routine_init(t_thread *th);

bool	check_bool(t_mtx *mutex, bool to_check);
bool	toggle_bool(t_mtx *mutex, bool *to_toggle, bool new_value);

int16_t	init_read_scene(t_scene *src, t_scene *dest);
int16_t	cpy_scene(t_scene *src, t_scene *dest);

#endif
