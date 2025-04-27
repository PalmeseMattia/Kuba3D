#include <cube_settings.h>

t_cube_settings	*settings_cube_init(t_map_config *map_config, t_tex_config *tex_config)
{
	t_cube_settings	*cube_settings;
	t_bool			valid;

	valid = TRUE;
	cube_settings = malloc(sizeof(t_cube_settings));
	if (!cube_settings)
		return (NULL);
	cube_settings->map_config = map_config;
	cube_settings->tex_config = tex_config;
	if (!map_config || !tex_config)
		valid = FALSE;
	cube_settings->valid = valid;
	return (cube_settings);
}

void	settings_cube_free(t_cube_settings *cube_settings)
{
	if (!cube_settings)
		return ;
	
}