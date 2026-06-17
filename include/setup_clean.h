#ifndef UTILS_H
# define UTILS_H
# include <scene.h>
# include <RTmlx.h>

int32_t		cleanup(t_rt *rt);

void		init_main(t_rt *rt, t_scene *scn, t_scene *read_scn, t_window *win);
int32_t		multithreaded(t_rt *rt);
int16_t 	setup_init_parsing(t_rt *rt, const int argc, const char *argv);

#endif
