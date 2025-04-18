#include <cube.h>
#include <cube_graphics.h>
#include <libft.h>

static void	close_cube(t_cube *cube)
{
	if (!cube)
		return ;
	cube->running = FALSE;
	if (cube->settings)
		free(cube->settings);
	if (cube->map)
		free(cube->map);
	if (cube->player)
		free(cube->player);
	if (cube->mlx_img && cube->mlx_img->img)
		mlx_destroy_image(cube->mlx, cube->mlx_img->img);
	if (cube->mlx_win)
		mlx_destroy_window(cube->mlx, cube->mlx_win);
	if (cube->mlx)
		mlx_destroy_display(cube->mlx);
}

static void close_error(t_cube *cube)
{
	ft_printf("Error initializing MLX\n");
	close_cube(cube);
	exit(EXIT_FAILURE);
}

static void	close_success(t_cube *cube)
{
	ft_printf("Gently Closing Cube\n");
	close_cube(cube);
	exit(EXIT_SUCCESS);
}

static void	initialize_mlx(t_cube *cube)
{
	t_image_data	img_data;

	cube->mlx = NULL;
	cube->mlx_img = NULL;
	cube->mlx_win = NULL;
	cube->mlx_img = &img_data;
	cube->mlx = mlx_init();
	if (!cube->mlx)
	{
		close_error(cube);
		return ;
	}
	cube->mlx_win = mlx_new_window(cube->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	if (!cube->mlx_win)
	{
		close_error(cube);
		return ;
	}
	cube->mlx_img->img = NULL;
}

void	initialize_game(t_cube *cube)
{
	cube->settings = load_mock_data();
	cube->map = load_mock_map();
	cube->player = load_mock_player();
	cube->time = 0;
	cube->old_time = 0;
	cube->running = TRUE;
	initialize_mlx(cube);
}

int game_loop_hook(t_cube *cube)
{
	t_scene_setup	scene;

	if (!cube->running)
	{
		close_success(cube);
		return (0);
	}
	scene = draw_scene(cube);
	cube->old_time = cube->time;
	cube->time = framerate_get_ticks();
	mov_handle_keypress(cube, scene, 0);
	return (0);
}