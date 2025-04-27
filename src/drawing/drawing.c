#include <cube.h>
#include <cube_mlx_handler.h>
#include <cube_dda.h>
#include <cube_entities.h>
#include <cube_drawing.h>
#include <cube_settings.h>
#include <mlx.h>
#include <math.h>
#include <stdio.h>
#include <cube_minimap.h>

void	draw_my_mlx_pixel_put(t_image_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

static double	draw_get_wall_x(t_cube *cube)
{
	t_dda_data	*dda;
	t_player	*player;
	double		wall_x;

	dda = cube->dda_data;
	player = cube->entities->player;
	if (dda->hit_type == HORIZONTAL)
        wall_x = player->y + dda->perp_wall_dist * dda->ray_dir.dir_y;
    else
        wall_x = player->x + dda->perp_wall_dist * dda->ray_dir.dir_x;
    wall_x -= floor(wall_x);
	return (wall_x);
}

static t_tex_type	draw_determine_face_hit(t_dda_data *dda)
{
    if (dda->hit_type == HORIZONTAL) {
        if (dda->step_x > 0)
            return TEX_TYPE_WEST;
        else
            return TEX_TYPE_EAST;
    } 
	else
	{
        if (dda->step_y > 0)
            return TEX_TYPE_NORTH;
        else
            return TEX_TYPE_SOUTH;
    }
}

static t_draw_vertical_slice_data	draw_prep_vertical_slice_data(t_cube *cube, int column_x)
{
	t_draw_vertical_slice_data	data;
	t_dda_data					*dda;

	dda = cube->dda_data;

	data.tex_type = 0;
	// TODO: Change this logic, handler only valid characters
	if (cube->map->tiles[dda->map_y][dda->map_x].c >= '1' && 
        cube->map->tiles[dda->map_y][dda->map_x].c <= '0' + TEXTURE_TYPES_COUNT)
		data.tex_type = (int)draw_determine_face_hit(dda);
    data.tex_x = (int)(draw_get_wall_x(cube) * (double)TEXTURE_SIZE);
    if(dda->hit_type == HORIZONTAL && dda->ray_dir.dir_x > 0)
        data.tex_x = TEXTURE_SIZE - data.tex_x - 1;
    if(dda->hit_type == VERTICAL && dda->ray_dir.dir_y < 0)
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

static void draw_textured(t_draw_vertical_slice_data *data, t_cube *cube, int y)
{
    int tex_y;
    unsigned int color;
    size_t **textures;
    t_dda_data *dda;

    dda = cube->dda_data;
    tex_y = (int)data->tex_pos & (TEXTURE_SIZE - 1);
    data->tex_pos += data->step;
    textures = cube->cube_settings->tex_config->textures;
    
    if (textures && data->tex_type >= 0 && data->tex_type < TEXTURE_TYPES_COUNT && 
        textures[data->tex_type] && tex_y >= 0 && tex_y < TEXTURE_SIZE && 
        data->tex_x >= 0 && data->tex_x < TEXTURE_SIZE)
    {
        color = textures[data->tex_type][TEXTURE_SIZE * tex_y + data->tex_x];
        // Darken the color for horizontal walls
        if (dda->hit_type == HORIZONTAL) 
            color = (color >> 1) & 8355711;
        draw_my_mlx_pixel_put(cube->mlx_handler->mlx_img, data->column_x, y, color);
    }
}


void draw_textured_vertical_slice(int column_x, t_cube *cube)
{
    t_draw_vertical_slice_data	data;
	int							y;
    
    data = draw_prep_vertical_slice_data(cube, column_x);
	y = (int)data.wall_top;
	if (y < 0)
		y = 0;
	while (y <= (int)data.wall_bottom && y < WINDOW_HEIGHT)
	{
		draw_textured(&data, cube, y);
		y++;
	}
}

void	draw_calculate_and_draw_single_stripe(int x, t_cube *cube)
{
	t_dda_data	*dda;
    double		camera_x;

	dda = cube->dda_data;
    camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
    dda_setup(cube, camera_x);
    dda_set_step_and_initial_side_dist(cube);
    dda_perform(cube);
    if (dda->hit_type == HORIZONTAL)
        dda->perp_wall_dist = (dda->side_dist_x - dda->delta_dist_x);
    else
        dda->perp_wall_dist = (dda->side_dist_y - dda->delta_dist_y);
	dda->wall_height = (int)(WINDOW_HEIGHT / dda->perp_wall_dist);
	cube->entities->buffer_z[x] = dda->perp_wall_dist;
    draw_textured_vertical_slice(x, cube);
}

static void draw_clear_screen(t_cube *cube)
{
    if (cube->mlx_handler->mlx_img->img != NULL) {
        mlx_destroy_image(cube->mlx_handler->mlx, cube->mlx_handler->mlx_img->img);
    }
    
    cube->mlx_handler->mlx_img->img = mlx_new_image(cube->mlx_handler->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    cube->mlx_handler->mlx_img->addr = mlx_get_data_addr(cube->mlx_handler->mlx_img->img, 
        &cube->mlx_handler->mlx_img->bits_per_pixel, 
        &cube->mlx_handler->mlx_img->line_length,
        &cube->mlx_handler->mlx_img->endian);
}

static t_draw_horizontal_data	draw_get_draw_horizontal_data(t_cube *cube, int y)
{
	t_dda_data				*dda;
	t_draw_horizontal_data	dhd;

	dda = cube->dda_data;
	dhd.ray_dir_x0 = dda->dir_vect.dir_x - dda->camera_dir.dir_x;
	dhd.ray_dir_y0 = dda->dir_vect.dir_y - dda->camera_dir.dir_y;
	dhd.ray_dir_x1 = dda->dir_vect.dir_x + dda->camera_dir.dir_x;
	dhd.ray_dir_y1 = dda->dir_vect.dir_y + dda->camera_dir.dir_y;

	// Current y position compared to the center of the screen (the horizon)
	dhd.pos_rel_to_horizon = y - WINDOW_HEIGHT / 2;

	// Vertical position of the camera.
	dhd.pos_vertical_z = 0.5 * WINDOW_HEIGHT;

	// Horizontal distance from the camera to the floor for the current row.
	// 0.5 is the z position exactly in the middle between floor and ceiling.
	dhd.row_distance = dhd.pos_vertical_z / dhd.pos_rel_to_horizon;

	// calculate the real world step vector we have to add for each x (parallel to camera plane)
	// adding step by step avoids multiplications with a weight in the inner loop
	dhd.floor_step_x= dhd.row_distance * (dhd.ray_dir_x1 - dhd.ray_dir_x0) / WINDOW_WIDTH;
	dhd.floor_step_y = dhd.row_distance * (dhd.ray_dir_y1 - dhd.ray_dir_y0) / WINDOW_WIDTH;

	// real world coordinates of the leftmost column. This will be updated as we step to the right.
	dhd.floor_x = dda->pos_x + dhd.row_distance * dhd.ray_dir_x0;
	dhd.floor_y = dda->pos_y + dhd.row_distance * dhd.ray_dir_y0;
	return (dhd);
}

static void	draw_floor_and_ceiling(t_cube *cube)
{
	t_draw_horizontal_data	dhd;
	int						y;
	int						x;
	size_t					**textures;

	y = -1;
	textures = cube->cube_settings->tex_config->textures;
	while (++y < WINDOW_HEIGHT)
	{
		dhd = draw_get_draw_horizontal_data(cube, y);
		x = -1;
		while (++x < WINDOW_WIDTH)
		{
			// the cell coord is simply got from the integer parts of floorX and floorY
			dhd.cell_x = (int)(dhd.floor_x);
			dhd.cell_y = (int)(dhd.floor_y);

			// get the texture coordinate from the fractional part
			dhd.texture_x = (int)(TEXTURE_SIZE * (dhd.floor_x - dhd.cell_x)) & (TEXTURE_SIZE - 1);
			dhd.texture_y = (int)(TEXTURE_SIZE * (dhd.floor_y - dhd.cell_y)) & (TEXTURE_SIZE - 1);

			dhd.floor_x += dhd.floor_step_x;
			dhd.floor_y += dhd.floor_step_y;

			
			dhd.color = textures[TEX_TYPE_FLOOR][TEXTURE_SIZE * dhd.texture_y + dhd.texture_x];
			dhd.color = (dhd.color >> 1) & 8355711; // make a bit darker
			draw_my_mlx_pixel_put(cube->mlx_handler->mlx_img, x, y, dhd.color);
			
			
			// ceiling (symmetrical, at screenHeight - y - 1 instead of y)
			dhd.color = textures[TEX_TYPE_CEILING][TEXTURE_SIZE * dhd.texture_y + dhd.texture_x];
			dhd.color = (dhd.color >> 1) & 8355711; // make a bit darker
			draw_my_mlx_pixel_put(cube->mlx_handler->mlx_img, x, WINDOW_HEIGHT - y - 1, dhd.color);
		}
	}
}

static void temp_prep_dda(t_cube *cube)
{
    t_player	*player;
    t_dda_data	*dda;

    player = cube->entities->player;
    dda = cube->dda_data;
    
    dda->pos_x = player->x;
    dda->pos_y = player->y;
    dda->dir_vect = player->dir;  // Copy the entire vector
    dda->camera_dir = player->camera;  // Copy the entire vector
}

void draw_scene(t_cube *cube)
{
    int x;

    if (!cube || !cube->entities || !cube->entities->player || !cube->dda_data || 
        !cube->mlx_handler || !cube->mlx_handler->mlx_img)
    {
        printf("Error: NULL pointers detected in draw_scene\n");
        return;
    }

    temp_prep_dda(cube);
    draw_clear_screen(cube);
	draw_floor_and_ceiling(cube);
    x = -1;
    while (++x < WINDOW_WIDTH)
        draw_calculate_and_draw_single_stripe(x, cube);
	sprites_draw(cube);
    draw_minimap(cube);
    mlx_put_image_to_window(cube->mlx_handler->mlx, cube->mlx_handler->mlx_win, 
                           cube->mlx_handler->mlx_img->img, 0, 0);
}