#include <cube.h>
#include <cube_settings.h>
#include <cube_entities.h>
#include <cube_input_handler.h>
#include <cube_mlx_handler.h>
#include <cube_runtime.h>
#include <cube_dda.h>
#include <stdio.h>

t_cube	*cube_cube_init(char **map, int width, int height, const char **tex_paths)
{
	t_cube	*cube;

	cube = malloc(sizeof(t_cube));
	if (!cube)
		return (NULL);
	printf("1... Allocated memory for cube\n");

	cube->mlx_handler = mlx_mlx_handler_init();
	printf("2... Initialized mlx_handler\n");

	cube->cube_settings = settings_cube_init(settings_map_config_init(map, width, height),
		settings_tex_config_init(tex_paths, cube->mlx_handler));
	printf("3... Initialized cube_settings\n");

	cube->entities = entities_entities_init(entities_entities_config_init(cube->cube_settings));
	printf("4... Initialized entities\n");

	cube->map = map_map_init(cube->cube_settings->map_config);
	printf("5... Initialized map\n");

	cube->runtime_handler = runtime_runtime_handler_init();
	printf("6... Initialized runtime_handler\n");

	cube->input_handler = input_handler_init();
	printf("7... Initialized input_handler\n");

	// TODO: Init DDA data
	printf("8... Initializing DDA data\n");
	cube->dda_data = dda_init();

	return (cube);
}

void	cube_cube_free(t_cube *cube)
{
	cube->runtime_handler->running = FALSE;
	map_map_free(cube->map);
	input_handler_free(cube->input_handler);
	entities_entities_free(cube->entities);
	settings_cube_free(cube->cube_settings);
	mlx_mlx_handler_free(cube->mlx_handler);
	runtime_runtime_handler_free(cube->runtime_handler);
	dda_free(cube->dda_data);
	safe_free(cube);
	exit(EXIT_SUCCESS);
}