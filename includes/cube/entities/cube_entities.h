#ifndef CUBE_ENTITIES_H
# define CUBE_ENTITIES_H

# include <utils.h>
# include <cube_map.h>
# include <stdlib.h>

typedef struct	s_keycard
{
	double	x;
	double	y;
	t_bool	is_picked_up;
	size_t	*tex;
}	t_keycard;

typedef struct	s_player
{
	double		x;
	double		y;
	t_vector	dir;
	t_vector	camera;
	int			hp;
	t_keycard	*keycard;
}	t_player;

typedef struct	s_enemy
{
	double	x;
	double	y;
	int		hp;
	size_t	*tex;
}	t_enemy;

typedef struct	s_entities
{
	t_player	*player;
	t_enemy		**enemies;
	t_keycard	*keycard;
}	t_entities;

typedef struct	s_entities_config
{
	t_point	player_location;
	t_point	keycard_location;
	size_t	*keycard_tex; 
	t_point	**enemies_locations;
	size_t	*enemy_tex;
}	t_entities_config;

typedef struct s_cube_settings	t_cube_settings;

t_entities			*entities_entities_init(t_entities_config config);
void				entities_entities_free(t_entities *entities);
t_entities_config	entities_entities_config_init(t_cube_settings *cube_settings);

#endif