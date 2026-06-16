#ifndef FORWARD_DECLARATIONS_H
# define FORWARD_DECLARATIONS_H

# include <pthread.h>

typedef struct	s_rt		t_rt;
typedef struct	s_scene		t_scene;
typedef struct	s_window	t_window;
typedef pthread_mutex_t		t_mtx;
typedef void				*(*t_cast)(void *);

#endif
