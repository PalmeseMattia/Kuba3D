#ifndef CUBE_MAP_H
# define CUBE_MAP_H

typedef struct	s_vector
{
	double	dir_x;
	double	dir_y;
}	t_vector;

typedef struct	s_point
{
	int		x;
	int		y;
	char	c;
}	t_point;

// Force the map to a rectangular shape
typedef struct	s_map
{
	t_point	**tiles;
	int		height;
	int		width;
}	t_map;

typedef struct	s_map	t_map;
typedef struct	s_cube	t_cube;

typedef struct s_map_config	t_map_config;

# include <utils.h>

t_point	map_point_init(int x, int y, char c);
t_point	*map_point_init_heap(int x, int y, char c);
t_map	*map_map_init(t_map_config *config);
void	map_map_free(t_map *map);
t_bool	map_is_within_bounds(int x, int y, t_map map);
t_bool	map_is_walkable(t_point pt);
t_bool	map_is_door_open(t_cube *cube);

#endif