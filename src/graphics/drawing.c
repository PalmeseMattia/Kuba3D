#include <cube_graphics.h>
#include <cube.h>
#include <math.h>
#include <libft.h>

void	draw_vertical_slice(int column_x, double wall_height, t_hit_type hit_type, mlx_image_t *mlx_img)
{
	double	wall_top;
	double	wall_bottom;
	int		y;

	wall_top = ((WINDOW_HEIGHT / 2.0) - (wall_height / 2.0));
	wall_bottom = ((WINDOW_HEIGHT / 2.0) + (wall_height / 2.0));
	y = -1;
	while (++y < WINDOW_HEIGHT)
	{
		if (y < wall_top)
			mlx_put_pixel(mlx_img, column_x, y, CEILING_COLOR);
		else if (y >= wall_top && y <= wall_bottom)
		{
			if (hit_type == VERTICAL)
				mlx_put_pixel(mlx_img, column_x, y, WALL_COLOR_DARK);
			else
				mlx_put_pixel(mlx_img, column_x, y, WALL_COLOR_LIGHT);
		}
		else
			mlx_put_pixel(mlx_img, column_x, y, FLOOR_COLOR);
	}
}

void	calculate_and_draw_single_stripe(int x, t_scene_setup *scene_setup, t_cube *cube, mlx_image_t *img)
{
		t_dda	dda;
		double camera_x;

		camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
		dda_init(&dda, scene_setup, camera_x);
		dda_set_step_and_initial_side_dist(&dda);
		dda_perform(&dda, cube);
		dda_set_wall_height(&dda);
		draw_vertical_slice(x, dda.wall_height, dda.hit_type, img);
}

void	draw_clear_screen(mlx_image_t *img)
{
	ft_memset(img->pixels, 255, img->width * img->height * sizeof(int32_t));
	// printf("Screen cleared ❌\n");
}

t_scene_setup	draw_scene(t_cube *cube)
{
	t_scene_setup	scene_setup;
	int				x;
	mlx_image_t		*img;

	img = cube->img;
	scene_setup.player_pos_x = cube->player->location.x;
	scene_setup.player_pos_y = cube->player->location.y;

	scene_setup.dir_vect.dir_x = cos(cube->player->angle);
	scene_setup.dir_vect.dir_y = sin(cube->player->angle);

	scene_setup.camera_plane_x = -scene_setup.dir_vect.dir_y * FOV;
	scene_setup.camera_plane_y = scene_setup.dir_vect.dir_x * FOV;

	draw_clear_screen(img);
	x = -1;
	while (++x < WINDOW_WIDTH)
		calculate_and_draw_single_stripe(x, &scene_setup, cube, img);
	// printf("Scene drawn ✅\n");
	return (scene_setup);
}