#ifndef CUBE_DRAWING_H
# define CUBE_DRAWING_H

# define DRAW_CEILING_COLOR 0xFF87CEEB

typedef struct	s_draw_horizontal_data
{
	// Global Parameters for each Frame
	float	ray_dir_x0;
	float	ray_dir_y0;

	float	ray_dir_x1;
	float	ray_dir_y1;

	int		pos_rel_to_horizon;
	
	float	pos_vertical_z;
	float	row_distance;

	float	floor_step_x;
	float	floor_step_y;

	float	floor_x;
	float	floor_y;

	// Individual Parameters for each Frame
	int		cell_x;
	int		cell_y;

	int		texture_x;
	int		texture_y;

	unsigned int	color;
}	t_draw_horizontal_data;

typedef struct	s_draw_vertical_slice_data
{
    int		y;
	double	step;
	double	wall_top;
    double	wall_bottom;
    int		tex_type;
	int		tex_x;
	double	tex_pos;
	int		column_x;
}	t_draw_vertical_slice_data;

typedef struct	s_cube	t_cube;

void	draw_scene(t_cube *cube);

#endif