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

void draw_textured_vertical_slice(int column_x, t_dda *dda, t_cube *cube)
{
    double wall_top;
    double wall_bottom;
    int y;
    int texNum = 0; // Default texture index, adjust as needed
    
    // Extract the texture number from the map character (assuming '1', '2', etc. represent different textures)
    if (cube->map->tiles[dda->map_y][dda->map_x].c >= '1' && 
        cube->map->tiles[dda->map_y][dda->map_x].c <= '0' + TEXTURES_COUNT)
        texNum = cube->map->tiles[dda->map_y][dda->map_x].c - '1';
    
    // Calculate the exact position where the wall was hit
    double wallX;
    if (dda->hit_type == HORIZONTAL)
        wallX = cube->player->location.y + dda->perp_wall_dist * dda->ray_dir_y;
    else
        wallX = cube->player->location.x + dda->perp_wall_dist * dda->ray_dir_x;
    wallX -= floor(wallX);
    
    // Calculate texture X coordinate
    int texX = (int)(wallX * (double)TEXTURE_SIZE);
    if(dda->hit_type == HORIZONTAL && dda->ray_dir_x > 0)
        texX = TEXTURE_SIZE - texX - 1;
    if(dda->hit_type == VERTICAL && dda->ray_dir_y < 0)
        texX = TEXTURE_SIZE - texX - 1;
    
    // Calculate wall height and drawing boundaries
    wall_top = ((WINDOW_HEIGHT / 2.0) - (dda->wall_height / 2.0));
    wall_bottom = ((WINDOW_HEIGHT / 2.0) + (dda->wall_height / 2.0));
    
    // Calculate the step to navigate through texture
    double step = 1.0 * TEXTURE_SIZE / dda->wall_height;
    // Starting texture coordinate
    double texPos = (wall_top < 0) ? -wall_top * step : 0;
    
    y = -1;
    while (++y < WINDOW_HEIGHT)
    {
        if (y < wall_top)
            my_mlx_pixel_put(cube->mlx_img, column_x, y, CEILING_COLOR);
        else if (y >= wall_top && y <= wall_bottom)
        {
            int texY = (int)texPos & (TEXTURE_SIZE - 1);
            texPos += step;
            
            // Check if the texture exists and is properly loaded
            if (cube->textures && cube->textures[texNum] && cube->textures[texNum]->texels)
            {
                unsigned int color = cube->textures[texNum]->texels[TEXTURE_SIZE * texY + texX];

                // Make color darker for horizontal hits
                if (dda->hit_type == HORIZONTAL) 
                    color = (color >> 1) & 8355711; // Reduces brightness by half
                
                my_mlx_pixel_put(cube->mlx_img, column_x, y, color);
            }
            else
            {
                // Fallback if texture not available
                if (dda->hit_type == VERTICAL)
                    my_mlx_pixel_put(cube->mlx_img, column_x, y, WALL_COLOR_DARK);
                else
                    my_mlx_pixel_put(cube->mlx_img, column_x, y, WALL_COLOR_LIGHT);
            }
        }
        else
            my_mlx_pixel_put(cube->mlx_img, column_x, y, FLOOR_COLOR);
    }
}

// Update the existing calculate_and_draw_single_stripe function to use the textured version
void calculate_and_draw_single_stripe(int x, t_scene_setup *scene_setup, t_cube *cube)
{
    t_dda dda;
    double camera_x;

    camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
    dda_init(&dda, scene_setup, camera_x);
    dda_set_step_and_initial_side_dist(&dda);
    dda_perform(&dda, cube);
    
    // Calculate perpendicular wall distance for proper texture mapping
    if (dda.hit_type == HORIZONTAL)
        dda.perp_wall_dist = (dda.side_dist_x - dda.delta_dist_x);
    else
        dda.perp_wall_dist = (dda.side_dist_y - dda.delta_dist_y);
    
    dda_set_wall_height(&dda);
    
    // Use textured drawing function
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

static void	clear_screen(t_cube *cube)
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
	clear_screen(cube);
	x = -1;
	while (++x < WINDOW_WIDTH)
		calculate_and_draw_single_stripe(x, &scene_setup, cube);
	mlx_put_image_to_window(cube->mlx, cube->mlx_win, cube->mlx_img->img, 0, 0);
	return (scene_setup);
}