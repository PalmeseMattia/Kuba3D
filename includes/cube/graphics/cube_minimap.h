#ifndef CUBE_MINIMAP_H
# define CUBE_MINIMAP_H

# define MINIMAP_SIZE 128
# define MINIMAP_PLAYER_ICON_SIZE 8
# define MINIMAP_ENEMY_ICON_SIZE 6
# define MINIMAP_BORDER_WIDTH 16
# define MINIMAP_CELLS_COUNT 15
# define MINIMAP_X 20
# define MINIMAP_Y 20
# define MINIMAP_CELL_SIZE 8
# define MINIMAP_BACKGROUND_COLOR 0x00000080
# define MINIMAP_BORDER_COLOR 0x333333FF
# define MINIMAP_WALL_COLOR 0xFFFFFFDD
# define MINIMAP_PLAYER_COLOR 0x0066FFFF
# define MINIMAP_DIRECTION_COLOR 0xFF3333FF
# define MINIMAP_PLAYER_RADIUS (MINIMAP_CELL_SIZE / 2)
# define MINIMAP_DIR_LEN (MINIMAP_CELL_SIZE * 1.5)

typedef struct	s_minimap_data
{
	int width;
	int height;
	int border_thickness;
	
	int center_x;
	int center_y;

	int start_x;
	int start_y;

	int end_x;
	int end_y;

	int		player_x;
	int		player_y;
	float	player_offset_x;
	float	player_offset_y;

	int		dir_x;
	int		dir_y;
	int		dir_length;
}	t_minimap_data;

typedef struct	s_minimap_cell_data
{
	int				cell_x;
	int				cell_y;
	unsigned int	color;
}	t_minimap_cell_data;

typedef struct	s_minimap_rect_data
{
	int				x;
	int				y;
	int				width;
	int				height;
	unsigned int	color;
}	t_minimap_rect_data;

typedef struct	s_minimap_circle_data
{
	int				center_x;
	int				center_y;
	int				radius;
	unsigned int	color;
}	t_minimap_circle_data;

typedef struct	s_minimap_bresenham_data
{
	int	dx;
    int	dy;
    int	sx;
    int	sy;
    int	err;
    int	e2;
}	t_minimap_bresenham_data;

typedef struct	s_minimap_draw_data
{
	int				x0;
	int				y0;
	int				x1;
	int				y1;
	unsigned int	color;
}	t_minimap_draw_data;


#endif