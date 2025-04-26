#include <new_cube.h>
#include <cube_settings.h>
#include <cube_entities.h>
#include <cube_input_handler.h>
#include <cube_mlx_handler.h>
#include <cube_runtime.h>

t_cube	*cube_cube_init(char **map, int width, int height, const char **tex_paths, t_mlx_handler *mlx_handler)
{
	t_cube	*cube;

	cube = malloc(sizeof(t_cube));
	if (!cube)
		return (NULL);
	cube->mlx_handler = mlx_mlx_handler_init();
	cube->cube_settings = settings_cube_init(settings_map_config_init(map, width, height),
		settings_tex_config_init(tex_paths, mlx_handler));
	cube->entities = entities_entities_init(entities_entities_config_init(cube->cube_settings));
	cube->input_handler = input_handler_init();
	cube->map = map_map_init(cube->cube_settings->map_config);
	cube->runtime_handler = runtime_runtime_handler_init();
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
	exit(EXIT_SUCCESS);
}