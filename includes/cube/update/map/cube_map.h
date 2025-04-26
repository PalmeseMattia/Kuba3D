#ifndef CUBE_MAP_H
# define CUBE_MAP_H

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

typedef struct s_map_config	t_map_config;

t_point	map_point_init(int x, int y, char c);
t_point	*map_point_init_heap(int x, int y, char c);
t_map	*map_map_init(t_map_config *config);
void	map_map_free(t_map *map);

#endif