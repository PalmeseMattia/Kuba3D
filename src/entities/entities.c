#include <stdlib.h>
#include <cube_entities.h>
#include <cube_settings.h>
#include <cube_mlx_handler.h>
#include <cube_animations.h>
#include <stdio.h>

t_keycard	*entities_keycard_init(t_point pt, size_t *tex)
{
	t_keycard	*keycard;

	if (pt.c == 0)
		return (NULL);
	keycard = malloc(sizeof(t_keycard));
	if (!keycard)
		return (NULL);
	keycard->is_picked_up = FALSE;
	keycard->tex = tex;
	keycard->x = pt.x;
	keycard->y = pt.y;
	return (keycard);
}

void	entities_keycard_free(t_keycard *keycard)
{
	if (!keycard)
		return ;
	free(keycard);
}

t_player *entities_player_init(t_point pt)
{
	t_player *player;

	player = malloc(sizeof(t_player));
	if (!player)
		return (NULL);
	player->hp = 100;
	player->keycard = NULL;
	player->x = pt.x + .5;
	player->y = pt.y + .5;

	// Initialize direction based on starting character
	if (pt.c == 'N')
	{
		player->dir.dir_x = 0;
		player->dir.dir_y = -1;
	}
	else if (pt.c == 'S')
	{
		player->dir.dir_x = 0;
		player->dir.dir_y = 1;
	}
	else if (pt.c == 'E')
	{
		player->dir.dir_x = 1;
		player->dir.dir_y = 0;
	}
	else if (pt.c == 'W')
	{
		player->dir.dir_x = -1;
		player->dir.dir_y = 0;
	}
	else
	{
		// Default to East if not specified
		player->dir.dir_x = 1;
		player->dir.dir_y = 0;
	}
	
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

t_enemy	*entities_enemy_init(t_point pt, size_t	*tex)
{
	t_enemy	*enemy;

	enemy = malloc(sizeof(t_enemy));
	if (!enemy)
		return (NULL);
	enemy->hp = 100;
	enemy->x = pt.x + .5;
	enemy->y = pt.y + .5;
	enemy->animation_controller = anim_animation_controller_init();
	anim_animation_controller_set_animation(enemy->animation_controller, ANIM_TYPE_IDLE, tex); // TODO: Change to idle_tex, attack_tex, ...
	return (enemy);
}

void	entities_enemy_free(t_enemy *enemy)
{
	if (!enemy)
		return ;
	free(enemy);
}

t_enemy		**entities_enemies_multiple_init(t_point **enemy_locations, size_t *tex)
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
		enemies[i] = entities_enemy_init(*enemy_locations[i], tex);
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

t_entities	*entities_entities_init(t_entities_config config)
{
	t_entities *entities;

	printf("Allocating memory for entities...\n");
	entities = malloc(sizeof(t_entities));
	if (!entities)
	{
		printf("Failed to allocate memory for entities.\n");
		return (NULL);
	}
	
	printf("Initializing enemies...\n");
	// Initialize enemies pointer to NULL by default
	entities->enemies = NULL;
	
	if (config.enemies_locations != NULL)
		entities->enemies = entities_enemies_multiple_init(config.enemies_locations, config.tex);
	else
		printf("No enemies found, skipping initialization...\n");

	printf("Initializing keycard...\n");
	entities->keycard = entities_keycard_init(config.keycard_location, config.keycard_tex);
	// if (!entities->keycard)
	// {
	// 	printf("Failed to initialize keycard.\n");
	// 	entities_enemies_multiple_free(entities->enemies);
	// 	free(entities);
	// 	return (NULL);
	// }
	printf("Initializing player...\n");
	entities->player = entities_player_init(config.player_location);
	if (!entities->player)
	{
		printf("Failed to initialize player.\n");
		entities_keycard_free(entities->keycard);
		entities_enemies_multiple_free(entities->enemies);
		free(entities);
		return (NULL);
	}
	printf("Entities initialized successfully.\n");
	return (entities);
}

void entities_entities_free(t_entities *entities)
{
	if (!entities)
		return;
	if (entities->enemies)
		entities_enemies_multiple_free(entities->enemies);
	if (entities->player)
		entities_player_free(entities->player);
	if (entities->keycard)
		entities_keycard_free(entities->keycard);
	safe_free(entities);
}

t_entities_config	entities_entities_config_init(t_cube_settings *cube_settings)
{
	t_entities_config	entities_config;

	printf("Initializing entities configuration...\n");
	printf("Setting enemies locations...\n");
	entities_config.enemies_locations = cube_settings->map_config->enemies_locations;
	entities_config.enemies_count = cube_settings->map_config->enemies_count;

	printf("Setting keycard location...\n");
	entities_config.keycard_location = cube_settings->map_config->key_location;

	printf("Setting player start location...\n");
	entities_config.player_location = cube_settings->map_config->start_location;

	printf("Setting enemy texture...\n");
	if (TEX_TYPE_ENEMY < TEXTURE_TYPES_COUNT && cube_settings->tex_config->textures[TEX_TYPE_ENEMY])
		entities_config.tex = cube_settings->tex_config->textures[TEX_TYPE_ENEMY];
	else
		entities_config.tex = NULL;

	printf("Setting keycard texture...\n");
	if (TEX_TYPE_KEYCARD < TEXTURE_TYPES_COUNT && cube_settings->tex_config->textures[TEX_TYPE_KEYCARD])
		entities_config.keycard_tex = cube_settings->tex_config->textures[TEX_TYPE_KEYCARD];
	else
		entities_config.keycard_tex = NULL;

	printf("Entities configuration initialized successfully.\n");
	return (entities_config);
}