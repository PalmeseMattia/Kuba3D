#include <cube_settings_map.h>
#include <stdlib.h>
#include <utils.h>
#include <stdio.h>

static size_t	settings_map_config_count_enemies(char **map, int width, int height)
{
	size_t	count;
	int		y;
	int		x;

	if (!map)
	{
		printf("Map is NULL. Returning count as 0.\n");
		return (0);
	}
	count = 0;
	y = -1;
	printf("Starting to count enemies in the map...\n");
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			if (map[y][x] && map[y][x] == 'X')
			{
				count++;
				printf("Enemy found at (%d, %d). Current count: %zu\n", x, y, count);
			}
		}
	}
	printf("Finished counting enemies. Total count: %zu\n", count);
	return (count);
}

t_point	**settings_map_config_get_enemies_locations(char **map, int width, int height)
{
	size_t	enemies_count;
	int		y;
	int		x;
	t_point	**enemies_locations;
	int		i;

	printf("Counting enemies in the map...\n");
	enemies_count = settings_map_config_count_enemies(map, width, height);
	printf("Enemies counted: %zu\n", enemies_count);
	if (!enemies_count)
	{
		printf("No enemies found. Returning NULL.\n");
		return (NULL);
	}

	printf("Allocating memory for enemies locations...\n");
	enemies_locations = malloc(sizeof(t_point *) * (enemies_count + 1));
	if (!enemies_locations)
	{
		printf("Failed to allocate memory for enemies locations. Returning NULL.\n");
		return (NULL);
	}

	y = -1;
	i = 0;
	printf("Iterating through the map to find enemies...\n");
	while (++y < height)
	{
		x = -1;
		while (++x < width)
		{
			if (map[y][x] && map[y][x] == 'X')
			{
				printf("Enemy found at (%d, %d). Initializing point...\n", x, y);
				enemies_locations[i++] = map_point_init_heap(x, y, map[y][x]);
				if (!enemies_locations[i - 1])
				{
					printf("Failed to initialize enemy point. Returning partially filled array.\n");
					return (enemies_locations);
				}
			}
		}
	}

	enemies_locations[i] = NULL;
	printf("All enemies locations initialized. Returning array.\n");
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
	map_config->key_location.c = 0;
	while (++y < map_config->height)
	{
		x = -1;
		while (++x < map_config->width)
		{
			c = map_config->map[y][x];
			if (settings_map_config_is_start(c))
			{
				printf("Start location found at (%d, %d) with character '%c'.\n", x, y, c);
				map_config->start_location = map_point_init(x, y, c);
			}
			else if (c == 'Q')
			{
				printf("Exit location found at (%d, %d) with character '%c'.\n", x, y, c);
				map_config->exit_location = map_point_init(x, y, c);
			}
			else if (c == 'K')
			{
				printf("Key location found at (%d, %d) with character '%c'.\n", x, y, c);
				map_config->key_location = map_point_init(x, y, c);
			}
			// Handle enemies
		}
	}
}

t_map_config	*settings_map_config_init(char **map, int width, int height)
{
	t_map_config	*map_config;

	printf("Allocating memory for map_config...\n");
	map_config = malloc(sizeof(t_map_config));
	if (!map_config)
	{
		printf("Failed to allocate memory for map_config.\n");
		return (NULL);
	}
	printf("Memory allocated for map_config.\n");

	printf("Getting enemies locations...\n");
	map_config->enemies_locations = settings_map_config_get_enemies_locations(map, width, height);
	printf("Enemies locations retrieved.\n");

	printf("Setting map dimensions...\n");
	map_config->height = height;
	map_config->width = width;
	printf("Map dimensions set: height=%d, width=%d.\n", height, width);

	printf("Assigning map pointer...\n");
	map_config->map = map;
	printf("Map pointer assigned.\n");

	printf("Extracting basic information from map...\n");
	settings_map_config_extract_basic_information(map_config);
	printf("Basic information extracted.\n");

	printf("Initialization complete. Returning map_config.\n");
	return (map_config);
}

void settings_map_config_free(t_map_config *map_config)
{
    int i;

    if (!map_config)
        return;
    if (map_config->enemies_locations)
    {
        i = -1;
        while (map_config->enemies_locations[++i])
            safe_free(map_config->enemies_locations[i]);
        safe_free(map_config->enemies_locations);
    }
    safe_free(map_config);
}