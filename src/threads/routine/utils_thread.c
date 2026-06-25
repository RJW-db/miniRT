#include <unistd.h>
#include <stdlib.h>
#include "libftx.h"
#include "miniRT.h"
#include "render.h"
#include "RTmlx.h"
#include "threadsRT.h"

bool	check_bool(t_mtx *mutex, bool to_check)
{
	bool	value;

	pthread_mutex_lock(mutex);
	value = to_check;
	pthread_mutex_unlock(mutex);
	return (value);
}

bool	toggle_bool(t_mtx *mutex, bool *to_toggle, bool new_value)
{
	pthread_mutex_lock(mutex);
	*to_toggle = new_value;
	pthread_mutex_unlock(mutex);
	return (new_value);
}
