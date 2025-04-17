#include <cube.h>
#include <utils.h>
#include <math.h>
#include <cube_graphics.h>

static void free_game(t_cube *cube)
{
	if (!cube)
		return ;
	if (cube->settings)
		free(cube->settings);
	if (cube->map)
		free(cube->map);
	if (cube->player)
		free(cube->player);
}

static void ft_error(t_cube *cube)
{
	free_game(cube);
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void initialize_game(t_cube *cube)
{
	mlx_t		*mlx;
	mlx_image_t	*img;

	cube->settings = load_mock_data();
	cube->map = load_mock_map();
	cube->player = load_mock_player();
	cube->time = 0;
	cube->old_time = 0;
	cube->running = true;
	mlx_set_setting(MLX_MAXIMIZED, WINDOW_RESIZE);
	mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, WINDOW_RESIZE);
	if (!mlx)
		ft_error(cube);
	img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error(cube);
	cube->img = img;
	cube->mlx = mlx;
}

static void game_loop_hook(void *param)
{
	t_cube			*cube;
	t_scene_setup	scene;

	cube = (t_cube *)param;
	if (!cube->running)
	{
		mlx_close_window(cube->mlx);
		return ;
	}
	scene = draw_scene(cube);
	mov_handle_keypress(cube, scene);	
}

int main(void)
{
	t_cube cube;

	initialize_game(&cube);
	print_map(*cube.map);
	mlx_loop_hook(cube.mlx, game_loop_hook, &cube);
	mlx_loop(cube.mlx);
	mlx_terminate(cube.mlx);
	free_game(&cube);
	return (EXIT_SUCCESS);
}