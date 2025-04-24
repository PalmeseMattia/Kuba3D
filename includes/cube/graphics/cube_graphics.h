#ifndef CUBE_GRAPHICS_H
# define CUBE_GRAPHICS_H

// # define CEILING_COLOR 0x000000FF
// # define FLOOR_COLOR 0x808080FF

# define CEILING_COLOR 0xFF000000
# define FLOOR_COLOR 0xFF000000

# define WALL_COLOR_LIGHT 0x880000FF
# define WALL_COLOR_DARK 0xFF8888FF

# define WINDOW_WIDTH 960
# define WINDOW_HEIGHT 640

# define TEXTURES_COUNT 4
# define TEXELS_COUNT (WINDOW_HEIGHT * WINDOW_WIDTH)
# define TEXTURE_SIZE 512

typedef struct	s_tex
{
	unsigned int	*texels;
}	t_tex;

typedef enum s_wall_face_hit
{
	WALL_FACE_NORTH,
	WALL_FACE_EAST,
	WALL_FACE_SOUTH,
	WALL_FACE_WEST
}	t_wall_face_hit;

typedef enum s_hit_type
{
	VERTICAL,
	HORIZONTAL
}	t_hit_type;

typedef struct	s_vect
{
	double	dir_x;
	double	dir_y;
}	t_vect;

typedef struct	s_scene_setup
{
	t_vect	dir_vect;

	double	player_pos_x;
	double	player_pos_y;

	double	camera_plane_x;	
	double	camera_plane_y;
}	t_scene_setup;

typedef struct s_dda
{
	t_hit_type	hit_type;

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
}	t_dda;

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

void			dda_set_wall_height(t_dda *dda);
void			dda_init(t_dda *dda, t_scene_setup *scene_setup, double camera_x);
void			dda_perform(t_dda *dda, t_cube *cube);
void			dda_set_step_and_initial_side_dist(t_dda *dda);
t_scene_setup	draw_scene(t_cube *cube);
double			framerate_get_ticks();
void			mov_handler(t_cube *cube);
int				game_loop_hook(t_cube *cube);
void			initialize_game(t_cube *cube);
t_scene_setup	draw_prep_scene(t_cube *cube);
void			close_cube(t_cube *cube);
int				on_destroy(t_cube *cube);
void			tex_load(const char **paths, t_cube *cube);

#endif