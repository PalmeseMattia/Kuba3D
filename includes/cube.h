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

typedef struct	s_cube
{
	t_settings	*settings;
	t_map		*map;
	t_player	*player;
	mlx_image_t	*img;

	size_t		time;
	size_t		old_time;
}	t_cube;

#endif