#include <cube_graphics.h>
#include <cube.h>
#include <math.h>
#include <libft.h>

void	my_mlx_pixel_put(t_image_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void draw_vertical_slice(int column_x, double wall_height, t_hit_type hit_type, t_cube *cube)
{
	double wall_top;
	double wall_bottom;
	int y;

	wall_top = ((WINDOW_HEIGHT / 2.0) - (wall_height / 2.0));
	wall_bottom = ((WINDOW_HEIGHT / 2.0) + (wall_height / 2.0));
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		if (y < wall_top)
			my_mlx_pixel_put(cube->mlx_img, column_x, y, CEILING_COLOR);
		else if (y >= wall_top && y <= wall_bottom)
		{
			if (hit_type == VERTICAL)
				my_mlx_pixel_put(cube->mlx_img, column_x, y, WALL_COLOR_DARK);
			else
			my_mlx_pixel_put(cube->mlx_img, column_x, y, WALL_COLOR_LIGHT);
		}
		else
			my_mlx_pixel_put(cube->mlx_img, column_x, y, FLOOR_COLOR);
	}
}

void calculate_and_draw_single_stripe(int x, t_scene_setup *scene_setup, t_cube *cube)
{
	t_dda	dda;
	double	camera_x;

	camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	dda_init(&dda, scene_setup, camera_x);
	dda_set_step_and_initial_side_dist(&dda);
	dda_perform(&dda, cube);
	dda_set_wall_height(&dda);
	draw_vertical_slice(x, dda.wall_height, dda.hit_type, cube);
}

t_scene_setup	draw_prep_scene(t_cube *cube)
{
	t_scene_setup scene_setup;

	scene_setup.player_pos_x = cube->player->location.x;
	scene_setup.player_pos_y = cube->player->location.y;
	scene_setup.dir_vect.dir_x = cos(cube->player->angle);
	scene_setup.dir_vect.dir_y = sin(cube->player->angle);
	scene_setup.camera_plane_x = -scene_setup.dir_vect.dir_y * FOV;
	scene_setup.camera_plane_y = scene_setup.dir_vect.dir_x * FOV;
	return (scene_setup);
}

static void	clear_screen(t_cube *cube)
{
	if (cube->mlx_img->img)
		mlx_destroy_image(cube->mlx, cube->mlx_img->img);
	cube->mlx_img->img = mlx_new_image(cube->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	cube->mlx_img->addr = mlx_get_data_addr(cube->mlx_img->img, &cube->mlx_img->bits_per_pixel, &cube->mlx_img->line_length,
		&cube->mlx_img->endian);
}

t_scene_setup draw_scene(t_cube *cube)
{
	t_scene_setup	scene_setup;
	int				x;

	scene_setup = draw_prep_scene(cube);
	clear_screen(cube);
	x = -1;
	while (++x < WINDOW_WIDTH)
		calculate_and_draw_single_stripe(x, &scene_setup, cube);
	mlx_put_image_to_window(cube->mlx, cube->mlx_win, cube->mlx_img->img, 0, 0);
	return (scene_setup);
}