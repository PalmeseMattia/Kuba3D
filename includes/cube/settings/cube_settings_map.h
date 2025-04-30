#ifndef CUBE_SETTINGS_MAP_H
# define CUBE_SETTINGS_MAP_H

# include <cube_map.h>

typedef struct	s_map_config
{
	int		width;
	int		height;
	t_point	exit_location;
	t_point	start_location;
	t_point	key_location;
	t_point	**enemies_locations;
	int		enemies_count;
	char	**map;
}	t_map_config;

t_map_config	*settings_map_config_init(char **map, int width, int height);
void			settings_map_config_free(t_map_config *map_config);

#endif