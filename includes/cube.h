#ifndef CUBE_H
# define CUBE_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include "MLX42/MLX42.h"
# include <cube_mock.h>

# define WINDOW_WIDTH 640
# define WINDOW_HEIGHT 480
# define FOV .66
# define TILE_SIZE 1
# define WINDOW_TITLE "Cub3D"
# define WINDOW_RESIZE FALSE

typedef enum s_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef struct	s_cube
{
	t_settings	*settings;
	t_map		*map;
	t_player	*player;
	mlx_image_t	*img;
	mlx_t		*mlx;

	size_t		time;
	size_t		old_time;
	t_bool		running;
}	t_cube;

#endif