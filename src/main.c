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

void	initialize_game(t_cube *cube)
{
	cube->settings = load_mock_data();
	cube->map = load_mock_map();
	cube->player = load_mock_player();
}

void	free_game(t_cube *cube)
{
	if (cube->settings)
		free(cube->settings);
	if (cube->map)
		free(cube->map);
	if (cube->player)
		free(cube->player);
}

double	dda_get_vertical_hit(t_cube *cube, double angle)
{
	t_dda	dda;

	dda.ray_dir_x = cos(angle);
	dda.ray_dir_y = sin(angle);

	if (dda.ray_dir_x > 0)
		dda.step_x = 1;
	else
		dda.step_x = -1;
	dda.next_x = floor(cube->player->location.x) + dda.step_x;

	dda.delta_x = dda.next_x - cube->player->location.x;
	dda.delta_y = dda.delta_x / dda.ray_dir_x * dda.ray_dir_y;
	dda.next_y = cube->player->location.y + dda.delta_y;

	dda.step_y = dda.step_x / dda.ray_dir_x * dda.ray_dir_y;

	t_point	pt;
	pt.x = dda.next_x;;
	pt.y = dda.next_y;;

	// int	vert_hit = 0;
	while (is_within_map_bounds(pt, *cube->map))
	{
		// Didn't get this part
		int	vx_sub_value;
		if (dda.ray_dir_y < 0)
			vx_sub_value = -1;
		else
			vx_sub_value = 0;
		if (cube->map->tiles[(int)pt.y][(int)pt.x + vx_sub_value].c == '1')
		{
			// vert_hit = 1;
			break;
		}
		pt.x += dda.step_x;
		pt.y += dda.step_y;
	}
	return hypot(pt.x - cube->player->location.x, pt.y - cube->player->location.y);
}

double	dda_get_horizontal_hit(t_cube *cube, double angle)
{
	t_dda	dda;

	angle = cube->player->angle;
	dda.ray_dir_x = cos(angle);
	dda.ray_dir_y = sin(angle);

	if (dda.ray_dir_y > 0)
		dda.step_y = 1;
	else
		dda.step_y = -1;
	dda.next_y = floor(cube->player->location.y) + dda.step_y;

	dda.delta_y = dda.next_y - cube->player->location.y;
	dda.delta_x = dda.delta_y / dda.ray_dir_y * dda.ray_dir_x;
	dda.next_x = cube->player->location.x + dda.delta_x;

	dda.step_x = dda.step_y / dda.ray_dir_y * dda.ray_dir_x;

	t_point	pt;
	pt.x = dda.next_x;;
	pt.y = dda.next_y;;

	// int	horiz_hit = 0;
	while (is_within_map_bounds(pt, *cube->map))
	{
		// Didn't get this part
		int	hy_sub_value;
		if (dda.ray_dir_y < 0)
			hy_sub_value = -1;
		else
			hy_sub_value = 0;
		if (cube->map->tiles[(int)pt.y + hy_sub_value][(int)pt.x].c == '1')
		{
			// horiz_hit = 1;
			break;
		}
		pt.x += dda.step_x;
		pt.y += dda.step_y;
	}
	return hypot(pt.x - cube->player->location.x, pt.y - cube->player->location.y);
}

t_dda	determine_hit(t_cube *cube, double ray_angle)
{
	t_dda	ret;

	double	dist_vert;
	double	dist_hor;

	dist_vert = dda_get_vertical_hit(cube, ray_angle);
	dist_hor = dda_get_horizontal_hit(cube, ray_angle);

	printf("Ray Angle: %f\n", ray_angle);
	printf("Vertical Distance: %f\n", dist_vert);
	printf("Horizontal Distance: %f\n", dist_hor);
	if (dist_vert < dist_hor)
	{
		ret.hit_type = VERTICAL;
		ret.total_dist = dist_vert;
	}
	else
	{
		ret.hit_type = HORIZONTAL;
		ret.total_dist = dist_hor;
	}
	return (ret);
}

void draw_vertical_slice(int column_x, int wall_height, t_hit_type hit_type, mlx_image_t *mlx_img)
{
	int wall_top = (WINDOW_HEIGHT / 2) - (wall_height / 2);
	int wall_bottom = (WINDOW_HEIGHT / 2) + (wall_height / 2);

	for (int y = 0; y < WINDOW_HEIGHT; y++) {
		if (y < wall_top) {
			mlx_put_pixel(mlx_img, column_x, y, CEILING_COLOR);
		} else if (y >= wall_top && y <= wall_bottom) {
			mlx_put_pixel(mlx_img, column_x, y, hit_type == VERTICAL ? WALL_COLOR_DARK : WALL_COLOR_LIGHT);
		} else {
			mlx_put_pixel(mlx_img, column_x, y, FLOOR_COLOR);
		}
	}
	
}


void	cast_ray(t_cube *cube, mlx_image_t *mlx_img)
{
	int	i;

	double ray_angle_start = cube->player->angle - (FOV / 2);
	// double ray_angle_end = cube->player->angle + (FOV / 2);
	double angle_step = FOV / WINDOW_WIDTH;

	i = -1;
	while (++i < WINDOW_WIDTH)
	{
		double ray_angle = ray_angle_start + i * angle_step;
		t_dda dda = determine_hit(cube, ray_angle);

		// Fix Fisheye ( TODO: )
		// corrected_distance = raw_distance * cos(ray_angle - pa)
		double corrected_distance = dda.total_dist;

		double projection_plane_distance = WINDOW_WIDTH / 2 / tan(FOV / 2);
		double wall_height = (TILE_SIZE / corrected_distance) * projection_plane_distance;
		
		// TODO: Change to correct hit type
		draw_vertical_slice(i, wall_height, dda.hit_type, mlx_img);
	}
}

int	main(void)
{
	t_cube	cube;

	initialize_game(&cube);

	print_map(*cube.map);
	mlx_set_setting(MLX_MAXIMIZED, false);
	mlx_t* mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D", false);
	if (!mlx)
		ft_error();

	// Create and display the image.
	mlx_image_t* img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error();	

	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	// mlx_loop_hook(mlx, ft_hook, mlx);
	cast_ray(&cube, img);
	mlx_loop(mlx);
	mlx_terminate(mlx);

	free_game(&cube);
	return (EXIT_SUCCESS);
}