#include <cube_graphics.h>
#include <cube.h>
#include <math.h>

// Helper function to draw a filled rectangle
static void draw_rectangle(t_image_data *img, 
                          int x, int y, 
                          int width, int height, 
                          unsigned int color)
{
    int i, j;
    
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (x + j >= 0 && x + j < WINDOW_WIDTH && 
                y + i >= 0 && y + i < WINDOW_HEIGHT)
            {
                draw_my_mlx_pixel_put(img, x + j, y + i, color);
            }
        }
    }
}

// Helper function to draw a line (Bresenham's algorithm)
static void draw_line(t_image_data *img, 
                      int x0, int y0, 
                      int x1, int y1, 
                      unsigned int color)
{
    int dx = abs(x1 - x0);
    int dy = -abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;
    
    while (1)
    {
        if (x0 >= 0 && x0 < WINDOW_WIDTH && y0 >= 0 && y0 < WINDOW_HEIGHT)
        {
            draw_my_mlx_pixel_put(img, x0, y0, color);
        }
        
        if (x0 == x1 && y0 == y1)
            break;
            
        e2 = 2 * err;
        if (e2 >= dy)
        {
            if (x0 == x1)
                break;
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            if (y0 == y1)
                break;
            err += dx;
            y0 += sy;
        }
    }
}

// Draw a circle using the midpoint circle algorithm
// static void draw_circle(t_image_data *img, 
//                        int center_x, int center_y, 
//                        int radius, 
//                        unsigned int color)
// {
//     int x = radius;
//     int y = 0;
//     int err = 0;

//     while (x >= y)
//     {
//         if (center_x + x >= 0 && center_x + x < WINDOW_WIDTH && 
//             center_y + y >= 0 && center_y + y < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x + x, center_y + y, color);
        
//         if (center_x + y >= 0 && center_x + y < WINDOW_WIDTH && 
//             center_y + x >= 0 && center_y + x < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x + y, center_y + x, color);
        
//         if (center_x - y >= 0 && center_x - y < WINDOW_WIDTH && 
//             center_y + x >= 0 && center_y + x < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x - y, center_y + x, color);
        
//         if (center_x - x >= 0 && center_x - x < WINDOW_WIDTH && 
//             center_y + y >= 0 && center_y + y < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x - x, center_y + y, color);
        
//         if (center_x - x >= 0 && center_x - x < WINDOW_WIDTH && 
//             center_y - y >= 0 && center_y - y < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x - x, center_y - y, color);
        
//         if (center_x - y >= 0 && center_x - y < WINDOW_WIDTH && 
//             center_y - x >= 0 && center_y - x < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x - y, center_y - x, color);
        
//         if (center_x + y >= 0 && center_x + y < WINDOW_WIDTH && 
//             center_y - x >= 0 && center_y - x < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x + y, center_y - x, color);
        
//         if (center_x + x >= 0 && center_x + x < WINDOW_WIDTH && 
//             center_y - y >= 0 && center_y - y < WINDOW_HEIGHT)
//             draw_my_mlx_pixel_put(img, center_x + x, center_y - y, color);
        
//         if (err <= 0)
//         {
//             y += 1;
//             err += 2 * y + 1;
//         }
//         else
//         {
//             x -= 1;
//             err -= 2 * x + 1;
//         }
//     }
// }

// Fill a circle
static void fill_circle(t_image_data *img, 
                       int center_x, int center_y, 
                       int radius, 
                       unsigned int color)
{
    int x, y;
    for (y = -radius; y <= radius; y++)
    {
        for (x = -radius; x <= radius; x++)
        {
            if (x*x + y*y <= radius*radius)
            {
                int draw_x = center_x + x;
                int draw_y = center_y + y;
                
                if (draw_x >= 0 && draw_x < WINDOW_WIDTH && 
                    draw_y >= 0 && draw_y < WINDOW_HEIGHT)
                {
                    draw_my_mlx_pixel_put(img, draw_x, draw_y, color);
                }
            }
        }
    }
}

void display_minimap(t_cube *cube)
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
    
    // Draw background and border
    draw_rectangle(cube->mlx_img, 
                  MINIMAP_X - border_thickness, 
                  MINIMAP_Y - border_thickness, 
                  minimap_width + 2 * border_thickness, 
                  minimap_height + 2 * border_thickness, 
                  BORDER_COLOR);
                  
    draw_rectangle(cube->mlx_img, 
                  MINIMAP_X, 
                  MINIMAP_Y, 
                  minimap_width, 
                  minimap_height, 
                  BACKGROUND_COLOR);
    
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
            
            // Draw cell with a small border to distinguish between cells
            draw_rectangle(cube->mlx_img, 
                          cell_x + 1, 
                          cell_y + 1, 
                          CELL_SIZE - 2, 
                          CELL_SIZE - 2, 
                          cell_color);
        }
    }
    
    // Calculate player position on minimap
    float player_offset_x = cube->player->location.x - start_x;
    float player_offset_y = cube->player->location.y - start_y;
    
    int player_x = MINIMAP_X + (int)(player_offset_x * CELL_SIZE);
    int player_y = MINIMAP_Y + (int)(player_offset_y * CELL_SIZE);
    
    // Draw player
    int player_radius = CELL_SIZE / 2;
    fill_circle(cube->mlx_img, player_x, player_y, player_radius, PLAYER_COLOR);
    
    // Draw player direction
    int dir_length = CELL_SIZE * 1.5;
    int dir_x = player_x + (int)(cos(cube->player->angle) * dir_length);
    int dir_y = player_y + (int)(sin(cube->player->angle) * dir_length);
    
    draw_line(cube->mlx_img, player_x, player_y, dir_x, dir_y, DIRECTION_COLOR);
}