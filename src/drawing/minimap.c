#include <cube_drawing.h>
#include <cube_minimap.h>
#include <cube_entities.h>
#include <cube.h>
#include <cube_mlx_handler.h>

void	minimap_draw_border(t_minimap_data *mmd)
{
	int	y;
	int	x;

	y = -1;
	while (++y < MINIMAP_SIZE + 2)
	{
		x = -1;
		while (++x < MINIMAP_SIZE + 2)
		{
			if (x == 0 || y == 0 || x == MINIMAP_SIZE + 1 || y == MINIMAP_SIZE + 1) {
				mmd->screen_x = mmd->minimap_pos_x - 1 + x;
				mmd->screen_y = mmd->minimap_pos_y - 1 + y;
				if (mmd->screen_x >= 0 && mmd->screen_x < WINDOW_WIDTH && \
						mmd->screen_y >= 0 && mmd->screen_y < WINDOW_HEIGHT)
					draw_my_mlx_pixel_put(mmd->img, mmd->screen_x, mmd->screen_y, MINIMAP_BORDER_COLOR);
			}
		}
	}
}

static void	minimap_draw_background(t_minimap_data *mmd)
{
	int	y;
	int	x;

	y = -1;
	while (++y < MINIMAP_SIZE)
	{
		x = -1;
		while (++x < MINIMAP_SIZE)
		{
			mmd->screen_x = mmd->minimap_pos_x + x;
			mmd->screen_y = mmd->minimap_pos_y + y;
			if (mmd->screen_x >= 0 && mmd->screen_x < WINDOW_WIDTH \
					&& mmd->screen_y >= 0 && mmd->screen_y < WINDOW_HEIGHT)
				draw_my_mlx_pixel_put(mmd->img, mmd->screen_x, 
					mmd->screen_y, MINIMAP_BACKGROUND_COLOR);
		}
	}
}

static void	minimap_draw_cells(t_minimap_data *mmd)
{
	int	y;
	int	x;
	t_cube *cube;

	cube = mmd->cube;
	y = -mmd->map_radius - 1;
	while (++y <= mmd->map_radius)
	{
		x = -mmd->map_radius - 1;
		while (++x <= mmd->map_radius)
		{
			mmd->map_x = mmd->map_center_x + x;
			mmd->map_y = mmd->map_center_y + y;
			
			// Skip if outside map bounds
			if (mmd->map_x < 0 || mmd->map_x >= cube->map->width || mmd->map_y < 0 || mmd->map_y >= cube->map->height)
				continue;
			
			// Calculate screen position
			mmd->screen_x = mmd->minimap_pos_x + (x + mmd->map_radius) * MINIMAP_CELL_SIZE;
			mmd->screen_y = mmd->minimap_pos_y + (y + mmd->map_radius) * MINIMAP_CELL_SIZE;
			
			// Draw map cell
			int color = MINIMAP_FLOOR_COLOR;
			if (cube->map->tiles[mmd->map_y][mmd->map_x].c == '1')
				color = MINIMAP_WALL_COLOR;
				
			for (int cy = 0; cy < MINIMAP_CELL_SIZE - 1; cy++) {
				for (int cx = 0; cx < MINIMAP_CELL_SIZE - 1; cx++) {
					int px = mmd->screen_x + cx;
					int py = mmd->screen_y + cy;
					if (px >= mmd->minimap_pos_x && px < mmd->minimap_pos_x + MINIMAP_SIZE && 
						py >= mmd->minimap_pos_y && py < mmd->minimap_pos_y + MINIMAP_SIZE)
						draw_my_mlx_pixel_put(mmd->img, px, py, color);
				}
			}
		}
	}
}

static void	minimap_draw_player(t_minimap_data *mmd)
{
	int	y;
	int	x;
	int px;
	int py;

	mmd->player_pos_x = mmd->minimap_pos_x + MINIMAP_SIZE / 2;
	mmd->player_pos_y = mmd->minimap_pos_y + MINIMAP_SIZE / 2;
	y = -MINIMAP_PLAYER_SIZE - 1;
	while (++y <= MINIMAP_PLAYER_SIZE)
	{
		x = -MINIMAP_PLAYER_SIZE - 1;
		while (++x <= MINIMAP_PLAYER_SIZE)
		{
			if (x * x + y * y <= MINIMAP_PLAYER_SIZE * MINIMAP_PLAYER_SIZE)
			{
				px = mmd->player_pos_x + x;
				py = mmd->player_pos_y + y;
				if (px >= mmd->minimap_pos_x && px < mmd->minimap_pos_x + MINIMAP_SIZE && 
					py >= mmd->minimap_pos_y && py < mmd->minimap_pos_y + MINIMAP_SIZE)
					draw_my_mlx_pixel_put(mmd->img, px, py, MINIMAP_PLAYER_COLOR);
			}
		}
	}
}

static void	minimap_bresenham_data_init(t_minimap_data *mmd)
{
	mmd->bd.dir_length = MINIMAP_PLAYER_SIZE * 3;
	mmd->bd.dir_end_x = mmd->player_pos_x + (int)(mmd->player->dir.dir_x * mmd->bd.dir_length);
	mmd->bd.dir_end_y = mmd->player_pos_y + (int)(mmd->player->dir.dir_y * mmd->bd.dir_length);	
	mmd->bd.dx = abs(mmd->bd.dir_end_x - mmd->player_pos_x);
	mmd->bd.dy = abs(mmd->bd.dir_end_y - mmd->player_pos_y);
	mmd->bd.sx = mmd->player_pos_x < mmd->bd.dir_end_x ? 1 : -1;
	mmd->bd.sy = mmd->player_pos_y < mmd->bd.dir_end_y ? 1 : -1;
	mmd->bd.err = mmd->bd.dx - mmd->bd.dy;
	mmd->bd.x = mmd->player_pos_x;
	mmd->bd.y = mmd->player_pos_y;
}

static void	minimap_draw_direction_line(t_minimap_data *mmd)
{
	int	e2;

	while (TRUE) 
	{
		if (mmd->bd.x >= mmd->minimap_pos_x && mmd->bd.x < mmd->minimap_pos_x + MINIMAP_SIZE && 
			mmd->bd.y >= mmd->minimap_pos_y && mmd->bd.y < mmd->minimap_pos_y + MINIMAP_SIZE)
			draw_my_mlx_pixel_put(mmd->img, mmd->bd.x, mmd->bd.y, MINIMAP_PLAYER_COLOR);
			
		if (mmd->bd.x == mmd->bd.dir_end_x && mmd->bd.y == mmd->bd.dir_end_y)
			break;
			
		e2 = 2 * mmd->bd.err;
		if (e2 > -mmd->bd.dy) {
			mmd->bd.err -= mmd->bd.dy;
			mmd->bd.x += mmd->bd.sx;
		}
		if (e2 < mmd->bd.dx) {
			mmd->bd.err += mmd->bd.dx;
			mmd->bd.y += mmd->bd.sy;
		}
	}
}

void draw_minimap(t_cube *cube)
{
	t_minimap_data	mmd;

	mmd.minimap_pos_x = WINDOW_WIDTH - MINIMAP_SIZE - MINIMAP_MARGIN;
	mmd.minimap_pos_y = MINIMAP_MARGIN;
	mmd.player = cube->entities->player;
	mmd.img = cube->mlx_handler->mlx_img;
	mmd.cube = cube;

	// Draw minimap border
	minimap_draw_border(&mmd);
	
	// Draw minimap background
	minimap_draw_background(&mmd);
	
	// Calculate the visible area around the player
	mmd.map_center_x = (int)mmd.player->x;
	mmd.map_center_y = (int)mmd.player->y;
	mmd.map_radius = MINIMAP_SIZE / (2 * MINIMAP_CELL_SIZE);
	
	// Draw map cells
	minimap_draw_cells(&mmd);
	
	// Draw player
	minimap_draw_player(&mmd);
	
	// Draw direction line
	minimap_bresenham_data_init(&mmd);
	minimap_draw_direction_line(&mmd);	
}