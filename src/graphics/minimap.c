#include <cube_graphics.h>
#include <cube.h>
#include <math.h>
#include <cube_minimap.h>

static void	draw_minimap_rectangle(t_image_data *img, t_minimap_rect_data rect_data)
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

static t_minimap_bresenham_data draw_minimap_get_bresenham_data(t_minimap_draw_data draw_data)
{
	t_minimap_bresenham_data	bresenham_data;

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
	t_minimap_bresenham_data	bd;

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

static void draw_minimap_fill_circle(t_image_data *img, t_minimap_circle_data cd)
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

static void	minimap_draw_border(t_minimap_data minimap_data, t_cube *cube)
{
	t_minimap_rect_data	dd;

	dd.x = MINIMAP_X - minimap_data.border_thickness;
	dd.y = MINIMAP_Y - minimap_data.border_thickness;
	dd.width = minimap_data.width + 2 * minimap_data.border_thickness;
	dd.height = minimap_data.height + 2 * minimap_data.border_thickness;
	dd.color = MINIMAP_BORDER_COLOR;
	draw_minimap_rectangle(cube->mlx_img, dd);
}

static void	minimap_draw_background(t_minimap_data minimap_data, t_cube *cube)
{
	t_minimap_rect_data	dd;

	dd.x = MINIMAP_X;
	dd.y = MINIMAP_Y;
	dd.width = minimap_data.width;
	dd.height = minimap_data.height;
	dd.color = MINIMAP_BACKGROUND_COLOR;
	draw_minimap_rectangle(cube->mlx_img, dd);
}

static void	minimap_draw_cell(t_cube *cube, t_minimap_cell_data cell)
{
	t_minimap_rect_data	dd;

	dd.x = cell.cell_x + 1;
	dd.y = cell.cell_y + 1;
	dd.width = MINIMAP_CELL_SIZE - 2;
	dd.height = MINIMAP_CELL_SIZE - 2;
	dd.color = cell.color;
	draw_minimap_rectangle(cube->mlx_img, dd);
}

static t_minimap_data	minimap_initialize_data(t_cube *cube)
{
	t_minimap_data	minimap_data;

	minimap_data.width = MINIMAP_CELLS_COUNT * MINIMAP_CELL_SIZE;
	minimap_data.height = MINIMAP_CELLS_COUNT * MINIMAP_CELL_SIZE;
	minimap_data.border_thickness = 2;
	minimap_data.center_x = (int)cube->player->location.x;
	minimap_data.center_y = (int)cube->player->location.y;
	minimap_data.start_x = minimap_data.center_x - MINIMAP_CELLS_COUNT / 2;
	minimap_data.start_y = minimap_data.center_y - MINIMAP_CELLS_COUNT / 2;
	minimap_data.end_x = minimap_data.start_x + MINIMAP_CELLS_COUNT;
	minimap_data.end_y = minimap_data.start_y + MINIMAP_CELLS_COUNT;
	minimap_data.player_offset_x = cube->player->location.x - minimap_data.start_x;
	minimap_data.player_offset_y = cube->player->location.y - minimap_data.start_y;
	minimap_data.player_x = MINIMAP_X + (int)(minimap_data.player_offset_x * MINIMAP_CELL_SIZE);
	minimap_data.player_y = MINIMAP_Y + (int)(minimap_data.player_offset_y * MINIMAP_CELL_SIZE);
	minimap_data.dir_x = minimap_data.player_x + (int)(cos(cube->player->angle) * MINIMAP_DIR_LEN);
	minimap_data.dir_y = minimap_data.player_y + (int)(sin(cube->player->angle) * MINIMAP_DIR_LEN);
	return (minimap_data);
}

static void	minimap_draw_cells(t_minimap_data minimap_data, t_cube *cube)
{
	t_minimap_cell_data	cell;

	for (int y = minimap_data.start_y; y < minimap_data.end_y; y++)
	{
		for (int x = minimap_data.start_x; x < minimap_data.end_x; x++)
		{
			// Skip cells outside map bounds
			if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE)
				continue;
				
			// Calculate cell position on minimap
			cell.cell_x = MINIMAP_X + (x - minimap_data.start_x) * MINIMAP_CELL_SIZE;
			cell.cell_y = MINIMAP_Y + (y - minimap_data.start_y) * MINIMAP_CELL_SIZE;			
			if (cube->map->tiles[y][x].c == '1')
				cell.color = MINIMAP_WALL_COLOR;
			else if (cube->map->tiles[y][x].c == 'E')
				cell.color = 0x00FF00CC;
			else
				cell.color = DRAW_FLOOR_COLOR;
			minimap_draw_cell(cube, cell);
		}
	}
}

static void	minimap_draw_player(t_minimap_data minimap_data, t_cube *cube)
{
	t_minimap_circle_data	cd;

	cd.center_x = minimap_data.player_x;
	cd.center_y = minimap_data.player_y;
	cd.radius = MINIMAP_PLAYER_RADIUS;
	cd.color = MINIMAP_PLAYER_COLOR;
	draw_minimap_fill_circle(cube->mlx_img, cd);
}

static void	minimap_draw_direction(t_minimap_data minimap_data, t_cube *cube)
{
	t_minimap_draw_data	mdd;

	mdd.x0 = minimap_data.player_x;
	mdd.y0 = minimap_data.player_y;
	mdd.x1 = minimap_data.dir_x;
	mdd.y1 = minimap_data.dir_y;
	mdd.color = MINIMAP_DIRECTION_COLOR;
	draw_minimap_line(cube->mlx_img, mdd);
}

void	display_minimap(t_cube *cube)
{
	t_minimap_data	minimap_data;

	if (!cube->mlx_img || !cube->mlx_img->img || !cube->mlx_img->addr)
		return;

	// Calculate minimap dimensions
	minimap_data = minimap_initialize_data(cube);
	
	// Draw background and border
	minimap_draw_border(minimap_data, cube);
	minimap_draw_background(minimap_data, cube);
	
	// Draw map cells
	minimap_draw_cells(minimap_data, cube);
		
	// Draw player
	minimap_draw_player(minimap_data, cube);
	
	// Draw direction line
	minimap_draw_direction(minimap_data, cube);
}