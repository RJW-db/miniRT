#include <scene.h>
#include <RTmlx.h>

//	Static Function
static void	update_filename_input(const keys_t key, t_window *win);

void	set_filename(const keys_t key, t_window *win, t_scene *sc)
{
	if (key == MLX_KEY_ENTER && win->file_creation == false)
	{
		win->file_creation = true;
		mlx_set_window_title(win->mlx, win->filename);
	}
	else if (key == MLX_KEY_ESCAPE)
	{
		mlx_set_window_title(win->mlx, "miniRT");
		reset_filename(win);
	}
	else if (key == MLX_KEY_ENTER && win->file_creation == true)
	{
		if (ft_strncmp(win->filename, NAME_FILE, sizeof(NAME_FILE)) != 0)
		{
			create_scene_rt_file(sc, win->filename + (sizeof(NAME_FILE) - 1));
			cpy_str0(win->filename + 6, "Created file: ");
			win->filename[sizeof(NAME_FILE) - 2] = ' ';
			// mlx_set_window_title(win->mlx, win->filename + 6);
		}
		mlx_set_window_title(win->mlx, "miniRT");
		reset_filename(win);
	}
	else
		update_filename_input(key, win);
}

void	reset_filename(t_window *win)
{
	win->filename_len = cpy_str0(win->filename, NAME_FILE);
	win->file_creation = false;
}

static void	update_filename_input(const keys_t key, t_window *win)
{
	if (key == MLX_KEY_BACKSPACE && win->filename_len > sizeof(NAME_FILE) - 1)
		win->filename[--win->filename_len] = '\0';
	else if (key >= MLX_KEY_SPACE && key <= MLX_KEY_GRAVE_ACCENT)
	{
		if (win->filename_len < FILE_CREATION + (sizeof(NAME_FILE) - 1))
		{
			if (key >= MLX_KEY_A && key <= MLX_KEY_Z)
				win->filename[win->filename_len++] = (char)(key + 32);
			else
				win->filename[win->filename_len++] = (char)key;
			win->filename[win->filename_len] = '\0';
		}
		else
			return (mlx_set_window_title(win->mlx, "Filename too long."));
	}
	mlx_set_window_title(win->mlx, win->filename);
}
