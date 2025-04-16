#include <cube.h>
#include <utils.h>
#include <math.h>
#include <cube_graphics.h>

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

// Print the window width and height.
// static void ft_hook(void* param)
// {
// 	const mlx_t* mlx = param;

// 	printf("WIDTH: %d | HEIGHT: %d\n", mlx->width, mlx->height);
// }

void initialize_game(t_cube *cube)
{
	cube->settings = load_mock_data();
	cube->map = load_mock_map();
	cube->player = load_mock_player();

	cube->time = 0;
	cube->old_time = 0;
}

void free_game(t_cube *cube)
{
	if (cube->settings)
		free(cube->settings);
	if (cube->map)
		free(cube->map);
	if (cube->player)
		free(cube->player);
}

static void game_loop_hook(void *param)
{
	t_cube			*cube;
	t_scene_setup	scene;

	cube = (t_cube *)param;
	scene = draw_scene(cube);

	// TODO: React to input
	cube->old_time = cube->time;
	cube->time = framerate_get_ticks();
	double frameTime = (cube->time - cube->old_time) / 1000.0; // frameTime is the time this frame has taken, in seconds
	printf("FPS: %f\n", 1.0 / frameTime);

	// Speed Modifiers
	double moveSpeed = frameTime * 5.0; // the constant value is in squares/second
	// double rotSpeed = frameTime * 3.0;	// the constant value is in radians/second

	t_point	pos;
	t_vect	dir;

	pos = cube->player->location;
	dir = scene.dir_vect;

	// Log the player's current position
	printf("Player Position: x = %f, y = %f\n", pos.x, pos.y);

	// Move forward if no wall in front of you
	if (mlx_is_key_down(cube->mlx, MLX_KEY_UP))
	{
		printf("Key Pressed: UP\n");
		if (cube->map->tiles[(int)(pos.x + dir.dir_x * moveSpeed)][(int)pos.y].c == '0')
			pos.x += dir.dir_x * moveSpeed;
		if (cube->map->tiles[(int)pos.x][(int)(pos.y + dir.dir_y * moveSpeed)].c == '0')
			pos.y += dir.dir_y * moveSpeed;
	}

	// Move backwards if no wall behind you
	if (mlx_is_key_down(cube->mlx, MLX_KEY_DOWN))
	{
		printf("Key Pressed: DOWN\n");
		if (cube->map->tiles[(int)(pos.x - dir.dir_x * moveSpeed)][(int)pos.y].c == '0')
			pos.x -= dir.dir_x * moveSpeed;
		if (cube->map->tiles[(int)pos.x][(int)(pos.y - dir.dir_y * moveSpeed)].c == '0')
			pos.y -= dir.dir_y * moveSpeed;
	}
	
	// Rotate to the right
	if (mlx_is_key_down(cube->mlx, MLX_KEY_RIGHT))
		cube->player->angle += .10;
	
	// Rotate to the left
	if (mlx_is_key_down(cube->mlx, MLX_KEY_LEFT))
		cube->player->angle -= .10;

	// printf("Player moved to location: x = %f, y = %f\n", pos.x, pos.y);
	cube->player->location.x = pos.x;
	cube->player->location.y = pos.y;
}

int main(void)
{
	t_cube cube;

	initialize_game(&cube);

	print_map(*cube.map);
	mlx_set_setting(MLX_MAXIMIZED, false);
	mlx_t *mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D", false);
	if (!mlx)
		ft_error();

	// Create and display the image.
	mlx_image_t *img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error();

	cube.img = img;
	cube.mlx = mlx;
	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_loop_hook(mlx, game_loop_hook, &cube);
	mlx_loop(mlx);
	mlx_terminate(mlx);

	free_game(&cube);
	return (EXIT_SUCCESS);
}