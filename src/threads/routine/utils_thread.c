#include <stdlib.h>
#include <miniRT.h>
#include <RTmlx.h>
#include <render.h>
#include <miniRT.h>
#include <threadsRT.h>
#include <unistd.h>
#include <libftx.h>

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

