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

static t_wall_face_hit draw_determine_face_hit(t_dda *dda)
{
    if (dda->hit_type == HORIZONTAL) {
        if (dda->step_x > 0)
            return WALL_FACE_WEST;
        else
            return WALL_FACE_EAST;
    } 
	else
	{
        if (dda->step_y > 0)
            return WALL_FACE_NORTH;
        else
            return WALL_FACE_SOUTH;
    }
}

static t_draw_vertical_slice_data	draw_prep_vertical_slice_data(t_dda *dda, t_cube *cube, int column_x)
{
	t_draw_vertical_slice_data	data;

	data.tex_num = 0;
	if (cube->map->tiles[dda->map_y][dda->map_x].c >= '1' && 
        cube->map->tiles[dda->map_y][dda->map_x].c <= '0' + TEXTURES_COUNT)
		data.tex_num = (int)draw_determine_face_hit(dda);
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
        // if (y < data.wall_top)
        //     draw_my_mlx_pixel_put(cube->mlx_img, column_x, y, CEILING_COLOR);
        // else
		if (y >= data.wall_top && y <= data.wall_bottom)
            draw_textured(&data, cube, dda, y);
        else if (y < data.wall_top)
            draw_my_mlx_pixel_put(cube->mlx_img, column_x, y, CEILING_COLOR);
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

static void	draw_floor_and_ceiling(t_cube *cube, t_scene_setup *scene)
{
	//FLOOR CASTING
	for(int y = 0; y < WINDOW_HEIGHT; y++)
	{
		// rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
		float rayDirX0 = scene->dir_vect.dir_x - scene->camera_plane_x;
		float rayDirY0 = scene->dir_vect.dir_y - scene->camera_plane_y;
		float rayDirX1 = scene->dir_vect.dir_x + scene->camera_plane_x;
		float rayDirY1 = scene->dir_vect.dir_y + scene->camera_plane_y;

		// Current y position compared to the center of the screen (the horizon)
		int p = y - WINDOW_HEIGHT / 2;

		// Vertical position of the camera.
		float posZ = 0.5 * WINDOW_HEIGHT;

		// Horizontal distance from the camera to the floor for the current row.
		// 0.5 is the z position exactly in the middle between floor and ceiling.
		float rowDistance = posZ / p;

		// calculate the real world step vector we have to add for each x (parallel to camera plane)
		// adding step by step avoids multiplications with a weight in the inner loop
		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WINDOW_WIDTH;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WINDOW_WIDTH;

		// real world coordinates of the leftmost column. This will be updated as we step to the right.
		float floorX = scene->player_pos_x + rowDistance * rayDirX0;
		float floorY = scene->player_pos_y + rowDistance * rayDirY0;

		for(int x = 0; x < WINDOW_WIDTH; ++x)
		{
			// the cell coord is simply got from the integer parts of floorX and floorY
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);

			// get the texture coordinate from the fractional part
			int tx = (int)(TEXTURE_SIZE * (floorX - cellX)) & (TEXTURE_SIZE - 1);
			int ty = (int)(TEXTURE_SIZE * (floorY - cellY)) & (TEXTURE_SIZE - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			// choose texture and draw the pixel
			unsigned int color;

			// floor
			color = cube->textures[FLOOR_TEXTURE]->texels[TEXTURE_SIZE * ty + tx];
			color = (color >> 1) & 8355711; // make a bit darker
			draw_my_mlx_pixel_put(cube->mlx_img, x, y, color);
			
			// ceiling (symmetrical, at screenHeight - y - 1 instead of y)
			// color = cube->textures[ceilingTexture][texWidth * ty + tx];
			// color = cube->textures[FLOOR_TEXTURE]->texels[TEXTURE_SIZE * ty + tx];
			// color = (color >> 1) & 8355711; // make a bit darker
			// draw_my_mlx_pixel_put(cube->mlx_img, x, WINDOW_HEIGHT - y - 1, color);
		}
	}
}

t_scene_setup draw_scene(t_cube *cube)
{
	t_scene_setup	scene_setup;
	int				x;

	scene_setup = draw_prep_scene(cube);
	draw_clear_screen(cube);
	x = -1;
	draw_floor_and_ceiling(cube, &scene_setup);
	while (++x < WINDOW_WIDTH)
		draw_calculate_and_draw_single_stripe(x, &scene_setup, cube);
	display_minimap(cube);
	mlx_put_image_to_window(cube->mlx, cube->mlx_win, cube->mlx_img->img, 0, 0);
	return (scene_setup);
}