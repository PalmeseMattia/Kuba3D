#include <cube.h>
#include <utils.h>
#include <math.h>
#include <cube_graphics.h>
#include <libft.h>

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
	// TODO: Properly close the MLX
	ft_printf("Error initializing MLX");
	exit(EXIT_FAILURE);
}

int	con_key_hook(int key_code, t_cube *cube)
{
	t_scene_setup	scene;

	printf("Hi dead\n");
	scene = draw_scene(cube);
	mov_handle_keypress(cube, scene, key_code);
	return (0);
}

static void initialize_game(t_cube *cube, t_image_data *image_data)
{
	cube->settings = load_mock_data();
	cube->map = load_mock_map();
	cube->player = load_mock_player();
	cube->time = 0;
	cube->old_time = 0;
	cube->running = TRUE;
	cube->mlx_img = image_data;
	cube->mlx = mlx_init();
	if (!cube->mlx)
	{
		ft_error(cube);
		return ;
	}
	cube->mlx_win = mlx_new_window(cube->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	if (!cube->mlx_win)
	{
		ft_error(cube);
		return ;
	}
	cube->mlx_img->img = NULL;
}

static int game_loop_hook(t_cube *cube)
{
	t_scene_setup	scene;

	if (!cube->running)
	{
		mlx_destroy_window(cube->mlx, cube->mlx_win);
		mlx_destroy_display(cube->mlx);
		free(cube->mlx);
		free_game(cube);
		exit(0);
		return (0);
	}
	scene = draw_scene(cube);
	cube->old_time = cube->time;
	cube->time = framerate_get_ticks();
	mov_handle_keypress(cube, scene, 0);
	return (0);
}

int main(void)
{
	t_cube			cube;
	t_image_data	img_data;

	initialize_game(&cube, &img_data);
	print_map(*cube.map);
	mlx_key_hook(cube.mlx_win, con_key_hook, &cube);
	mlx_loop_hook(cube.mlx, game_loop_hook, &cube);
	mlx_loop(cube.mlx);
	return (EXIT_SUCCESS);
}