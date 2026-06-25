#include "miniRT.h"
#include "render.h"
#include "RTerror.h"
#include "setup_clean.h"

int	main(int argc, char **argv)
{
	t_rt		rt;
	t_scene		sc;
	t_scene		read_sc;
	t_window	win;

	init_main(&rt, &sc, &read_sc, &win);
	if (setup_init_parsing(&rt, argc, argv[1]) != EXIT_SUCCESS)
		return ((int)errset(ERTRN));
	init_hooks(&rt);
	if (THREADS > 1)
	{
		rt.scene->render_ongoing = true;
		if (multithreaded(&rt) != EXIT_SUCCESS)
		{
			cleanup(&rt);
			return ((int)errset(ERTRN));
		}
	}
	mlx_loop(rt.win->mlx);
	cleanup(&rt);
	printf("exiting miniRT\n");
	return (EXIT_SUCCESS);
}
