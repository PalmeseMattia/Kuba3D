#include <stdlib.h>
#include <cube_entities.h>
#include <cube_settings.h>
#include <cube_mlx_handler.h>
#include <cube_animations.h>
#include <libft.h>

t_entity	*entities_entity_init(t_point pt, t_entity_type type, t_animated_frames *frames_ptr)
{
	t_entity	*entity;
	t_location	location;

	entity = malloc(sizeof(t_entity));
	if (!entity)
		return (NULL);
	if (type == ENTITY_TYPE_PLAYER)
		entity->controller = NULL;
	else
	{
		entity->controller = anim_animation_controller_init();
		anim_animation_controller_set_animation(entity->controller, ANIM_TYPE_IDLE, frames_ptr);
		entity->controller->playing = FALSE;
		entity->controller->repeat = FALSE;
		entity->controller->current = entity->controller->idle;
	}
	entity->hp = 100;
	location.x = pt.x + ENTITY_CENTRAL_OFFSET;
	location.y = pt.y + ENTITY_CENTRAL_OFFSET;
	entity->current_location = location;
	return (entity);
}

void	entities_entity_free(t_entity *entity)
{
	if (!entity)
		return ;
	anim_animation_controller_free(entity->controller);
	safe_free(entity);
}

static t_vector	entities_player_get_orientation(t_point pt)
{
	t_vector	ret;

	if (pt.c == 'N')
	{
		ret.dir_x = 0;
		ret.dir_y = -1;
	}
	else if (pt.c == 'S')
	{
		ret.dir_x = 0;
		ret.dir_y = 1;
	}
	else if (pt.c == 'E')
	{
		ret.dir_x = 1;
		ret.dir_y = 0;
	}
	else
	{
		ret.dir_x = -1;
		ret.dir_y = 0;
	}
	return (ret);
}

t_player *entities_player_init(t_point pt)
{
	t_player *player;

	player = malloc(sizeof(t_player));
	if (!player)
		return (NULL);
	player->base = entities_entity_init(pt, ENTITY_TYPE_PLAYER, NULL);
	if (!player->base)
	{
		safe_free(player);
		return (NULL);
	}
	player->dir = entities_player_get_orientation(pt);

	// Initialize camera plane perpendicular to direction
	player->camera.dir_x = player->dir.dir_y * FOV;
	player->camera.dir_y = -player->dir.dir_x * FOV;
	
	return (player);
}

void	entities_player_free(t_player *player)
{
	if (!player)
		return ;
	free(player);
}

t_enemy	*entities_enemy_init(t_point pt, t_animated_frames *frames_ptr)
{
	t_enemy	*enemy;

	enemy = malloc(sizeof(t_enemy));
	if (!enemy)
		return (NULL);
	enemy->base = entities_entity_init(pt, ENTITY_TYPE_ENEMY, frames_ptr);
	if (!enemy->base)
	{
		safe_free(enemy);
		return (NULL);
	}
	enemy->base->controller->playing = TRUE;
	enemy->base->controller->repeat = TRUE;
	return (enemy);
}

void entities_enemy_free(t_enemy *enemy)
{
	if (!enemy)
		return;
	entities_entity_free(enemy->base);
	safe_free(enemy);
}

t_exit	*entities_exit_init(t_point pt, t_animated_frames *idle_frames_ptr, t_animated_frames *open_frames_ptr)
{
	t_exit	*ret;

	ret = malloc(sizeof(t_exit));
	if (!ret)
		return (NULL);
	ret->base = entities_entity_init(pt, ENTITY_TYPE_EXIT, idle_frames_ptr);
	if (!ret->base)
	{
		safe_free(ret);
		return (NULL);
	}
	anim_animation_controller_set_animation(ret->base->controller, ANIM_TYPE_OPEN, open_frames_ptr);
	ret->unlocked = FALSE;
	return (ret);
}

void entities_exit_free(t_exit *exit_entity)
{
	if (!exit_entity)
		return;
	entities_entity_free(exit_entity->base);
	safe_free(exit_entity);
}

t_enemy		**entities_enemies_multiple_init(t_point **enemy_locations, t_animated_frames *frames_ptr)
{
	t_enemy	**enemies;
	int		i;

	i = -1;
	if (!enemy_locations)
		return (NULL);
	while (enemy_locations[++i] != NULL)
		;
	enemies = malloc(sizeof(t_enemy *) * (i + 1));
	if (!enemies)
		return (NULL);
	i = -1;
	while (enemy_locations[++i] != NULL)
		enemies[i] = entities_enemy_init(*enemy_locations[i], frames_ptr);
	enemies[i] = NULL;
	return (enemies);
}

void entities_enemies_multiple_free(t_enemy **enemies)
{
	int i;

	if (!enemies)
		return;
	
	i = -1;
	// Loop through enemies until NULL is found
	while (enemies[++i] != NULL)
		entities_enemy_free(enemies[i]);
	
	safe_free(enemies);
	enemies = NULL;
}

t_entities *entities_entities_init(t_entities_config config)
{
	t_entities *entities;
	int num_enemies = 0;

	printf("Allocating memory for entities...\n");
	entities = malloc(sizeof(t_entities));
	if (!entities)
	{
		printf("Failed to allocate memory for entities.\n");
		return (NULL);
	}

	
	printf("Initializing exit...\n");
	entities->exit = entities_exit_init(config.exit_location, config.exit_idle_frames_ptr, config.exit_open_frames_ptr);

	printf("Initializing enemies...\n");
	// Initialize enemies pointer to NULL by default
	entities->enemies = NULL;
	
	if (config.enemies_locations != NULL)
		entities->enemies = entities_enemies_multiple_init(config.enemies_locations, config.enemy_frames_ptr);
	else
		printf("No enemies found, skipping initialization...\n");

	printf("Initializing player...\n");
	entities->player = entities_player_init(config.player_location);
	if (!entities->player)
	{
		printf("Failed to initialize player.\n");
		entities_enemies_multiple_free(entities->enemies);
		free(entities);
		return (NULL);
	}

	// Allocate memory for sprite arrays
	num_enemies = config.enemies_count;
	if (num_enemies > 0)
	{
		entities->sprite_order = malloc(sizeof(int) * num_enemies);
		entities->sprite_distance = malloc(sizeof(double) * num_enemies);
		
		if (!entities->sprite_order || !entities->sprite_distance)
		{
			entities_entities_free(entities);
			return (NULL);
		}
	}
	else
	{
		entities->sprite_order = NULL;
		entities->sprite_distance = NULL;
	}
	return entities;
}

void entities_entities_free(t_entities *entities)
{
	if (!entities)
		return;
	entities_enemies_multiple_free(entities->enemies);
	entities_player_free(entities->player);
	entities_exit_free(entities->exit);
	safe_free(entities->sprite_order);
	safe_free(entities->sprite_distance);
	safe_free(entities);
}

t_entities_config	entities_entities_config_init(t_cube_settings *cube_settings)
{
	t_entities_config	entities_config;

	entities_config.enemies_locations = cube_settings->map_config->enemies_locations;
	entities_config.enemies_count = cube_settings->map_config->enemies_count;
	entities_config.player_location = cube_settings->map_config->start_location;
	entities_config.enemy_frames_ptr = cube_settings->tex_config->enemy_frames;
	entities_config.exit_idle_frames_ptr = cube_settings->tex_config->exit_idle_frames;
	entities_config.exit_open_frames_ptr = cube_settings->tex_config->exit_open_frames;
	entities_config.exit_location = cube_settings->map_config->exit_location;
	ft_printf("Entities configuration initialized successfully.\n");
	return (entities_config);
}