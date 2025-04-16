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

void draw_vertical_slice(int column_x, double wall_height, t_hit_type hit_type, mlx_image_t *mlx_img)
{
	double wall_top = ((WINDOW_HEIGHT / 2.0) - (wall_height / 2.0));
	double wall_bottom = ((WINDOW_HEIGHT / 2.0) + (wall_height / 2.0));

	for (int y = 0; y < WINDOW_HEIGHT; y++)
	{
		if (y < wall_top)
		{
			mlx_put_pixel(mlx_img, column_x, y, CEILING_COLOR);
		}
		else if (y >= wall_top && y <= wall_bottom)
		{
			mlx_put_pixel(mlx_img, column_x, y, hit_type == VERTICAL ? WALL_COLOR_DARK : WALL_COLOR_LIGHT);
		}
		else
		{
			mlx_put_pixel(mlx_img, column_x, y, FLOOR_COLOR);
		}
	}
}

void set_step_and_initial_side_dist(t_dda *dda)
{
	if (dda->ray_dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (dda->pos_x - dda->map_x) * dda->delta_dist_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_dist_x = (dda->map_x + 1.0 - dda->pos_x) * dda->delta_dist_x;
	}
	if (dda->ray_dir_y < 0)
	{
		dda->step_y = -1;
		dda->side_dist_y = (dda->pos_y - dda->map_y) * dda->delta_dist_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_dist_y = (dda->map_y + 1.0 - dda->pos_y) * dda->delta_dist_y;
	}
}

void perform_dda(t_dda *dda, t_cube *cube)
{
	int hit;

	hit = 0;
	while (hit == 0)
	{
		if (dda->side_dist_x < dda->side_dist_y)
		{
			dda->side_dist_x += dda->delta_dist_x;
			dda->map_x += dda->step_x;
			dda->hit_type = HORIZONTAL;
		}
		else
		{
			dda->side_dist_y += dda->delta_dist_y;
			dda->map_y += dda->step_y;
			dda->hit_type = VERTICAL;
		}

		// Check if ray has hit a wall
		t_point pt;
		pt.x = dda->map_x;
		pt.y = dda->map_y;
		if (!is_within_map_bounds(pt, *cube->map))
		{
			hit = 1;
			break;
		}
		if (cube->map->tiles[dda->map_y][dda->map_x].c == '1')
			hit = 1;
	}
}

void set_wall_height(t_dda *dda)
{
	double perp_wall_dist;

	if (dda->hit_type == HORIZONTAL)
		perp_wall_dist = (dda->side_dist_x - dda->delta_dist_x);
	else
		perp_wall_dist = (dda->side_dist_y - dda->delta_dist_y);

	int wall_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
	dda->wall_height = wall_height;
}

void draw_initial_scene(t_cube *cube, mlx_image_t *img)
{
	double posX = cube->player->location.x, posY = cube->player->location.y;

	// TODO: Remove artificial offset once controls work
	cube->player->angle -= 0.2;

	double dirX = cos(cube->player->angle);
	double dirY = sin(cube->player->angle);

	double planeX = -dirY * FOV;
	double planeY = dirX * FOV;

	for (int x = 0; x < WINDOW_WIDTH; x++)
	{
		t_dda dda;
		// calculate ray position and direction
		double cameraX = 2 * x / (double)WINDOW_WIDTH - 1;

		dda.ray_dir_x = dirX + planeX * cameraX;
		dda.ray_dir_y = dirY + planeY * cameraX;

		// which box of the map we're in
		dda.pos_x = posX;
		dda.pos_y = posY;

		dda.map_x = (int)posX;
		dda.map_y = (int)posY;

		dda.delta_dist_x = (dda.ray_dir_x == 0) ? 1e30 : fabs(1 / dda.ray_dir_x);
		dda.delta_dist_y = (dda.ray_dir_y == 0) ? 1e30 : fabs(1 / dda.ray_dir_y);

		// double perpWallDist;

		set_step_and_initial_side_dist(&dda);

		// perform DDA
		perform_dda(&dda, cube);

		set_wall_height(&dda);

		// draw the pixels of the stripe as a vertical line
		draw_vertical_slice(x, dda.wall_height, dda.hit_type == 1 ? HORIZONTAL : VERTICAL, img);
	}
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

	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	// mlx_loop_hook(mlx, ft_hook, mlx);
	draw_initial_scene(&cube, img);
	mlx_loop(mlx);
	mlx_terminate(mlx);

	free_game(&cube);
	return (EXIT_SUCCESS);
}