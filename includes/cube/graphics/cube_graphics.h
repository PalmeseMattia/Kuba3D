#ifndef CUBE_GRAPHICS_H
# define CUBE_GRAPHICS_H

# define CEILING_COLOR 0x00FF00FF
# define FLOOR_COLOR 0x0000FFFF

# define WALL_COLOR_LIGHT 0xFF8888FF
# define WALL_COLOR_DARK 0x880000FF

# include <cube.h>

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

	int pos_x;
	int pos_y;

	int map_x;
	int map_y;

	// Drawing
	double	wall_height;
}	t_dda;

typedef struct	s_image_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_image_data;

void	dda_set_wall_height(t_dda *dda);
void	dda_init(t_dda *dda, t_scene_setup *scene_setup, double camera_x);
void	dda_perform(t_dda *dda, t_cube *cube);
void	dda_set_step_and_initial_side_dist(t_dda *dda);
void	draw_scene(t_cube *cube, mlx_image_t *img);

#endif