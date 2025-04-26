#include <cube_settings_map.h>
#include <stdlib.h>
#include <utils.h>

static size_t	settings_map_config_count_enemies(char **map, int width, int height)
{
	size_t	count;
	int		y;
	int		x;

	if (!map)
		return (0);
	count = 0;
	y = -1;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			if (map[y][x] && map[y][x] == 'X')
				count++;
		}
	}
	return (count);
}

t_point	**settings_map_config_get_enemies_locations(char **map, int width, int height)
{
	size_t	enemies_count;
	int		y;
	int		x;
	t_point	**enemies_locations;
	int		i;

	enemies_count = settings_map_config_count_enemies(map, width, height);
	if (!enemies_count)
		return (NULL);
	enemies_locations = malloc(sizeof(t_point *) * (enemies_count + 1));
	if (!enemies_locations)
		return (NULL);
	y = -1;
	i = 0;
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			if (map[y][x] && map[y][x] == 'X')
			{
				enemies_locations[i++] = map_point_init_heap(x, y, map[y][x]);
				if (!enemies_locations[i - 1])
					return (enemies_locations);
			}
		}
	}
	enemies_locations[i] = NULL;
	return (enemies_locations);
}

static t_bool	settings_map_config_is_start(char c)
{
	return (c == 'E' || c == 'W' || c == 'S' || c == 'N');
}

void	settings_map_config_extract_basic_information(t_map_config *map_config)
{
	int				y;
	int				x;
	char			c;

	if (!map_config || !map_config->map)
		return ;
	y = -1;
	while (++y < map_config->height)
	{
		x = -1;
		while (++x < map_config->width)
		{
			c = map_config->map[y][x];
			if (settings_map_config_is_start(c))
				map_config->start_location = map_point_init(x, y, c);
			else if (c == 'Q')
				map_config->exit_location = map_point_init(x, y, c);
			else if (c == 'K')
				map_config->key_location = map_point_init(x, y, c);
			// Handle enemies
		}
	}
}

t_map_config	*settings_map_config_init(char **map, int width, int height)
{
	t_map_config	*map_config;

	map_config = malloc(sizeof(t_map_config));
	if (!map_config)
		return (NULL);
	map_config->enemies_locations = settings_map_config_get_enemies_locations(map, width, height);
	map_config->height = height;
	map_config->width = width;
	map_config->map = map;
	settings_map_config_extract_basic_information(map_config);
	return (map_config);
}