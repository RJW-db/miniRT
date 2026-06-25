#ifndef MINIRT_H
# define MINIRT_H

# define _GNU_SOURCE

# include <stdbool.h>
# include <stdint.h>
# include "forward_declarations.h"
# include "threadsRT.h"

# ifndef SCREEN_WIDTH
#  define SCREEN_WIDTH 5120
# endif

# ifndef SCREEN_HEIGHT
#  define SCREEN_HEIGHT 2700
# endif

# ifndef THREADS
#  define THREADS 1
# endif

struct	s_rt
{
	struct s_scene	*scene;
	struct s_scene	*read_scene;
	struct s_window	*win;
	pthread_cond_t	cond;
	int16_t			stopped_threads;
	uint16_t		finished_rendering;
	uint16_t		thread_rend_step;
	bool			creation_check;
	bool			mtx_init_check;
	bool			quit_routine;
	int				errnum;
	t_mtx			mtx[MTX_AMOUNT];
	t_thread		thread;
};

#endif
