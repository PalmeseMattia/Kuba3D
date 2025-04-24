#include <cube_graphics.h>
#include <cube.h>
#include <math.h>

static void	draw_minimap_rectangle(t_image_data *img, t_rect_data rect_data)
{
    int	i;
	int	j;
	int offset_x;
	int offset_y;

	i = -1;
	while (++i < rect_data.height)
	{
		j = -1;
		while (++j < rect_data.width)
		{
			offset_x = rect_data.x + j;
			offset_y = rect_data.y + i;
			if (offset_x >= 0 && offset_x < WINDOW_WIDTH && 
                offset_y >= 0 && offset_y < WINDOW_HEIGHT)
                draw_my_mlx_pixel_put(img, offset_x, offset_y, rect_data.color);
		}
	}
}

static t_bresenham_data draw_minimap_get_bresenham_data(t_minimap_draw_data draw_data)
{
	t_bresenham_data	bresenham_data;

	bresenham_data.dx = abs(draw_data.x1 - draw_data.x0);
    bresenham_data.dy = -abs(draw_data.y1 - draw_data.y0);
	if (draw_data.x0 < draw_data.x1)
		bresenham_data.sx = 1;
	else
		bresenham_data.sx = -1;
	if (draw_data.y0 < draw_data.y1)
		bresenham_data.sy = 1;
	else
		bresenham_data.sy = -1;
    bresenham_data.err = bresenham_data.dx + bresenham_data.dy;
	return (bresenham_data);
}

static void draw_minimap_line(t_image_data *img, t_minimap_draw_data draw_data)
{
    t_bresenham_data	bd;

	bd = draw_minimap_get_bresenham_data(draw_data);
    while (TRUE)
    {
        if (draw_data.x0 >= 0 && draw_data.x0 < WINDOW_WIDTH && draw_data.y0 >= 0 && draw_data.y0 < WINDOW_HEIGHT)
            draw_my_mlx_pixel_put(img, draw_data.x0, draw_data.y0, draw_data.color);        
        if (draw_data.x0 == draw_data.x1 && draw_data.y0 == draw_data.y1)
            break;
        bd.e2 = 2 * bd.err;
        if (bd.e2 >= bd.dy)
        {
            if (draw_data.x0 == draw_data.x1)
                break;
			bd.err += bd.dy;
            draw_data.x0 += bd.sx;
        }
        if (bd.e2 <= bd.dx)
        {
            if (draw_data.y0 == draw_data.y1)
                break;
			bd.err += bd.dx;
            draw_data.y0 += bd.sy;
        }
    }
}

static void draw_minimap_fill_circle(t_image_data *img, t_circle_data cd)
{
    int	x;
	int	y;
	int	draw_x;
	int	draw_y;

	y = -cd.radius - 1;
	while (++y <= cd.radius)
	{
		x = -cd.radius - 1;
		while (++x <= cd.radius)
		{
			if (x * x + y * y <= cd.radius * cd.radius)
            {
                draw_x = cd.center_x + x;
                draw_y = cd.center_y + y;
                if (draw_x >= 0 && draw_x < WINDOW_WIDTH && 
                    draw_y >= 0 && draw_y < WINDOW_HEIGHT)
                    draw_my_mlx_pixel_put(img, draw_x, draw_y, cd.color);
            }
		}
	}
}

void	display_minimap(t_cube *cube)
{
    // Configuration constants
    const int MINIMAP_X = 20;  // Top-left corner X position
    const int MINIMAP_Y = 20;  // Top-left corner Y position
    const int CELL_SIZE = 8;   // Size of each map cell in pixels
    const int MAP_DISPLAY_SIZE = 15; // Number of cells to show in each direction
    
    // Skip minimap if image data is NULL
    if (!cube->mlx_img || !cube->mlx_img->img || !cube->mlx_img->addr)
        return;
    
    // Colors
    const unsigned int BACKGROUND_COLOR = 0x00000080;  // Semi-transparent black
    const unsigned int BORDER_COLOR = 0x333333FF;      // Dark gray
    const unsigned int WALL_COLOR = 0xFFFFFFDD;        // White with some transparency
    const unsigned int PLAYER_COLOR = 0x0066FFFF;      // Blue
    const unsigned int DIRECTION_COLOR = 0xFF3333FF;   // Red
    
    // Calculate minimap dimensions
    int minimap_width = MAP_DISPLAY_SIZE * CELL_SIZE;
    int minimap_height = MAP_DISPLAY_SIZE * CELL_SIZE;
    int border_thickness = 2;
    
    // Calculate the center cell (player position)
    int center_x = (int)cube->player->location.x;
    int center_y = (int)cube->player->location.y;
    
    // Define the region of the map to display
    int start_x = center_x - MAP_DISPLAY_SIZE / 2;
    int start_y = center_y - MAP_DISPLAY_SIZE / 2;
    int end_x = start_x + MAP_DISPLAY_SIZE;
    int end_y = start_y + MAP_DISPLAY_SIZE;

	t_rect_data	dd;
    
    // Draw background and border
	dd.x = MINIMAP_X - border_thickness;
	dd.y = MINIMAP_Y - border_thickness;
	dd.width = minimap_width + 2 * border_thickness;
	dd.height = minimap_height + 2 * border_thickness;
	dd.color = BORDER_COLOR;
    draw_minimap_rectangle(cube->mlx_img, dd);
	
	dd.x = MINIMAP_X;
	dd.y = MINIMAP_Y;
	dd.width = minimap_width;
	dd.height = minimap_height;
	dd.color = BACKGROUND_COLOR;
    draw_minimap_rectangle(cube->mlx_img, dd);
    
    // Draw map cells
    for (int y = start_y; y < end_y; y++)
    {
        for (int x = start_x; x < end_x; x++)
        {
            // Skip cells outside map bounds
            if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
                continue;
            
            // Calculate cell position on minimap
            int cell_x = MINIMAP_X + (x - start_x) * CELL_SIZE;
            int cell_y = MINIMAP_Y + (y - start_y) * CELL_SIZE;
            
            // Draw cell based on map content
            unsigned int cell_color;
            
            if (cube->map->tiles[y][x].c == '1')
                cell_color = WALL_COLOR;
            else if (cube->map->tiles[y][x].c == 'E')
                cell_color = 0x00FF00CC;  // Green for player start
            else
                cell_color = FLOOR_COLOR;
            
            dd.x = cell_x + 1;
            dd.y = cell_y + 1;
			dd.width = CELL_SIZE - 2;
			dd.height = CELL_SIZE - 2;
			dd.color = cell_color;
            draw_minimap_rectangle(cube->mlx_img, dd);
        }
    }
    
    // Calculate player position on minimap
    float player_offset_x = cube->player->location.x - start_x;
    float player_offset_y = cube->player->location.y - start_y;
    
    int player_x = MINIMAP_X + (int)(player_offset_x * CELL_SIZE);
    int player_y = MINIMAP_Y + (int)(player_offset_y * CELL_SIZE);
    
    // Draw player
    int player_radius = CELL_SIZE / 2;
	t_circle_data	cd;
	cd.center_x = player_x;
	cd.center_y = player_y;
	cd.radius = player_radius;
	cd.color = PLAYER_COLOR;
    draw_minimap_fill_circle(cube->mlx_img, cd);
    
    // Draw player direction
    int dir_length = CELL_SIZE * 1.5;
    int dir_x = player_x + (int)(cos(cube->player->angle) * dir_length);
    int dir_y = player_y + (int)(sin(cube->player->angle) * dir_length);
    
	t_minimap_draw_data	mdd;
	mdd.x0 = player_x;
	mdd.y0 = player_y;
	mdd.x1 = dir_x;
	mdd.y1 = dir_y;
	mdd.color = DIRECTION_COLOR;
    draw_minimap_line(cube->mlx_img, mdd);
}