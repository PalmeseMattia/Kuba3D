#ifndef CUBE_H
# define CUBE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "MLX42/MLX42.h"
#include <cube_mock.h>

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540

typedef struct	s_cube
{
	t_settings	*settings;
	t_map		*map;
	t_player	*player;
}	t_cube;

#endif