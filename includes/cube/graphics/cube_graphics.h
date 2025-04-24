#ifndef CUBE_GRAPHICS_H
# define CUBE_GRAPHICS_H

# define DRAW_CEILING_COLOR 0xFF87CEEB
# define DRAW_FLOOR_COLOR 0x808080FF

# define WALL_COLOR_LIGHT 0x880000FF
# define WALL_COLOR_DARK 0xFF8888FF

# define WINDOW_WIDTH 960
# define WINDOW_HEIGHT 640

typedef enum	s_drawing_type
{
	NONE,
	FLOOR,
	CEILING,
	FLOOR_AND_CEILING
}	t_drawing_type;
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

typedef enum s_dda_wall_face_hit
{
	WALL_FACE_NORTH,
	WALL_FACE_EAST,
	WALL_FACE_SOUTH,
	WALL_FACE_WEST
}	t_wall_face_hit;

typedef enum s_dda_hit_type
{
	VERTICAL,
	HORIZONTAL
}	t_dda_hit_type;

typedef struct	s_dda_vector
{
	double	dir_x;
	double	dir_y;
}	t_dda_vector;

typedef struct	s_draw_scene_data
{
	t_dda_vector	dir_vect;

	double	player_pos_x;
	double	player_pos_y;

	double	camera_plane_x;	
	double	camera_plane_y;
}	t_draw_scene_data;

typedef struct s_dda_data
{
	t_dda_hit_type	hit_type;

	double	ray_dir_x;
	double	ray_dir_y;

	double	step_x;
	double	step_y;

	double	side_dist_x;
	double	side_dist_y;

	double	delta_dist_x;
	double	delta_dist_y;

	double pos_x;
	double pos_y;

	int map_x;
	int map_y;

	// Drawing
	double	wall_height;
	double	perp_wall_dist;

	// Wall face
	t_wall_face_hit	wall_face_hit;
}	t_dda_data;

typedef struct	s_draw_vertical_slice_data
{
    int		y;
	double	step;
	double	wall_top;
    double	wall_bottom;
    int		tex_num;
	int		tex_x;
	double	tex_pos;
	int		column_x;
}	t_draw_vertical_slice_data;

typedef struct	s_cube t_cube;
typedef struct	s_image_data t_image_data;

void				dda_set_wall_height(t_dda_data *dda);
void				dda_init(t_dda_data *dda, t_draw_scene_data *scene_setup, double camera_x);
void				dda_perform(t_dda_data *dda, t_cube *cube);
void				dda_set_step_and_initial_side_dist(t_dda_data *dda);
t_draw_scene_data	draw_scene(t_cube *cube);
double				framerate_get_ticks();
void				mov_handler(t_cube *cube);
int					game_loop_hook(t_cube *cube);
void				initialize_game(t_cube *cube);
t_draw_scene_data	draw_prep_scene(t_cube *cube);
void				close_cube(t_cube *cube);
int					on_destroy(t_cube *cube);
void				tex_load(const char **paths, t_cube *cube);
void				display_minimap(t_cube *cube);
void				draw_my_mlx_pixel_put(t_image_data *data, int x, int y, int color);

#endif