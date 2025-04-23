#include <cube_graphics.h>
#include <cube.h>
#include <math.h>
#include <libft.h>

void	draw_my_mlx_pixel_put(t_image_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

static double	draw_get_wall_x(t_dda *dda, t_cube *cube)
{
	double	wall_x;

	if (dda->hit_type == HORIZONTAL)
        wall_x = cube->player->location.y + dda->perp_wall_dist * dda->ray_dir_y;
    else
        wall_x = cube->player->location.x + dda->perp_wall_dist * dda->ray_dir_x;
    wall_x -= floor(wall_x);
	return (wall_x);
}

static t_draw_vertical_slice_data	draw_prep_vertical_slice_data(t_dda *dda, t_cube *cube, int column_x)
{
	t_draw_vertical_slice_data	data;

	data.tex_num = 0;
	if (cube->map->tiles[dda->map_y][dda->map_x].c >= '1' && 
        cube->map->tiles[dda->map_y][dda->map_x].c <= '0' + TEXTURES_COUNT)
        data.tex_num = cube->map->tiles[dda->map_y][dda->map_x].c - '1';
    data.tex_x = (int)(draw_get_wall_x(dda, cube) * (double)TEXTURE_SIZE);
    if(dda->hit_type == HORIZONTAL && dda->ray_dir_x > 0)
        data.tex_x = TEXTURE_SIZE - data.tex_x - 1;
    if(dda->hit_type == VERTICAL && dda->ray_dir_y < 0)
        data.tex_x = TEXTURE_SIZE - data.tex_x - 1;
    data.wall_top = ((WINDOW_HEIGHT / 2.0) - (dda->wall_height / 2.0));
    data.wall_bottom = ((WINDOW_HEIGHT / 2.0) + (dda->wall_height / 2.0));
    data.step = 1.0 * TEXTURE_SIZE / dda->wall_height;
	if (data.wall_top < 0)
		data.tex_pos = -data.wall_top * data.step;
	else
		data.tex_pos = 0;
	data.column_x = column_x;
	return (data);
}

static void	draw_textured(t_draw_vertical_slice_data *data, t_cube *cube, t_dda *dda, int y)
{
	int				tex_y;
	unsigned int	color;

	tex_y = (int)data->tex_pos & (TEXTURE_SIZE - 1);
	data->tex_pos += data->step;
	if (cube->textures && cube->textures[data->tex_num] && cube->textures[data->tex_num]->texels)
	{
		color = cube->textures[data->tex_num]->texels[TEXTURE_SIZE * tex_y + data->tex_x];
		if (dda->hit_type == HORIZONTAL) 
			color = (color >> 1) & 8355711;
		draw_my_mlx_pixel_put(cube->mlx_img, data->column_x, y, color);
	}
}

void draw_textured_vertical_slice(int column_x, t_dda *dda, t_cube *cube)
{
    t_draw_vertical_slice_data	data;
	int							y;
    
    data = draw_prep_vertical_slice_data(dda, cube, column_x);
    y = -1;
    while (++y < WINDOW_HEIGHT)
    {
        if (y < data.wall_top)
            draw_my_mlx_pixel_put(cube->mlx_img, column_x, y, CEILING_COLOR);
        else if (y >= data.wall_top && y <= data.wall_bottom)
            draw_textured(&data, cube, dda, y);
        else
            draw_my_mlx_pixel_put(cube->mlx_img, column_x, y, FLOOR_COLOR);
    }
}

void	draw_calculate_and_draw_single_stripe(int x, t_scene_setup *scene_setup, t_cube *cube)
{
    t_dda	dda;
    double	camera_x;

    camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
    dda_init(&dda, scene_setup, camera_x);
    dda_set_step_and_initial_side_dist(&dda);
    dda_perform(&dda, cube);
    if (dda.hit_type == HORIZONTAL)
        dda.perp_wall_dist = (dda.side_dist_x - dda.delta_dist_x);
    else
        dda.perp_wall_dist = (dda.side_dist_y - dda.delta_dist_y);
    dda_set_wall_height(&dda);
    draw_textured_vertical_slice(x, &dda, cube);
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

static void	draw_clear_screen(t_cube *cube)
{
	if (cube->mlx_img->img != NULL)
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
	draw_clear_screen(cube);
	x = -1;
	while (++x < WINDOW_WIDTH)
		draw_calculate_and_draw_single_stripe(x, &scene_setup, cube);
	mlx_put_image_to_window(cube->mlx, cube->mlx_win, cube->mlx_img->img, 0, 0);
	return (scene_setup);
}