#ifndef CUBE_DDA_H
# define CUBE_DDA_H

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

# include <cube_map.h>

typedef struct s_dda_data
{
	t_dda_hit_type	hit_type;
	t_vector		camera_dir;
	t_vector		ray_dir;
	t_vector		dir_vect;

	double			step_x;
	double			step_y;

	double			side_dist_x;
	double			side_dist_y;

	double			delta_dist_x;
	double			delta_dist_y;

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

typedef struct	s_cube	t_cube;

void		dda_free(t_dda_data *dda);
t_dda_data	*dda_init();
void		dda_setup(t_cube *cube, double camera_x);
void		dda_set_step_and_initial_side_dist(t_cube *cube);
void		dda_perform(t_cube *cube);
void		dda_set_wall_height(t_cube *cube);

#endif