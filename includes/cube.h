#ifndef CUBE_H
# define CUBE_H

typedef struct	s_cube_settings	t_cube_settings;
typedef struct	s_map	t_map;
typedef struct	s_entities	t_entities;
typedef struct	s_mlx_handler	t_mlx_handler;
typedef struct	s_runtime_handler	t_runtime_handler;
typedef struct	s_input_handler	t_input_handler;
typedef struct	s_dda_data	t_dda_data;

typedef struct	s_cube
{
	t_cube_settings		*cube_settings;
	t_map				*map;
	t_entities			*entities;
	t_mlx_handler		*mlx_handler;
	t_runtime_handler	*runtime_handler;
	t_input_handler		*input_handler;
	t_dda_data			*dda_data;
}	t_cube;

t_cube	*cube_cube_init(char **map, int width, int height, const char **tex_paths);
void	cube_cube_free(t_cube *cube);

#endif
