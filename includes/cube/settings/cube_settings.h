#ifndef CUBE_SETTINGS_H
# define CUBE_SETTINGS_H

# include <cube_settings_textures.h>
# include <cube_settings_map.h>
# include <utils.h>

typedef struct	s_cube_settings
{
	t_map_config	*map_config;
	t_tex_config	*tex_config;
	t_bool			valid;
}	t_cube_settings;

t_cube_settings	*settings_cube_init(t_map_config *map_config, t_tex_config *tex_config);
void			settings_cube_free(t_cube_settings *cube_settings);

#endif