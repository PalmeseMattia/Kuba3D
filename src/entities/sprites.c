#include <cube_entities.h>
#include <cube_settings.h>
#include <cube_settings_map.h>
#include <cube.h>
#include <cube_drawing.h>
#include <cube_animations.h>
#include <cube_settings_animated_sprites.h>
#include <math.h>
#include <stdlib.h>

void sprites_sort(int *order, double *dist, int amount)
{
    // Your existing sorting function looks good
    for (int i = 0; i < amount - 1; i++) {
        for (int j = 0; j < amount - i - 1; j++) {
            if (dist[j] < dist[j + 1]) {
                // Swap distances
                double tempDist = dist[j];
                dist[j] = dist[j + 1];
                dist[j + 1] = tempDist;

                // Swap orders
                int tempOrder = order[j];
                order[j] = order[j + 1];
                order[j + 1] = tempOrder;
            }
        }
    }
}

// Count how many enemies we have
static int count_enemies(t_enemy **enemies)
{
    int count = 0;
    if (!enemies)
        return 0;
        
    while (enemies[count] != NULL)
        count++;
    
    return count;
}

// Getting the distance^2, but it doesn't matter in this case
static double	sprite_enemy_get_distance(t_enemy *enemy, t_player *player)
{
	return ((player->base->current_location.x - enemy->base->current_location.x) * (player->base->current_location.x - enemy->base->current_location.x) + 
                             (player->base->current_location.y - enemy->base->current_location.y) * (player->base->current_location.y - enemy->base->current_location.y));
}

void sprites_draw(t_cube *cube)
{
    t_entities	*entities;
    t_player	*player;
    t_enemy		**enemies;

    int			num_sprites;
    int			*sprite_order;
    double		*sprite_distance;

	size_t		*tex;
    
    // Get entities and player pointers
    entities = cube->entities;
    player = entities->player;
    enemies = entities->enemies;
    
    // Count sprites (enemies)
    num_sprites = count_enemies(enemies);
    if (num_sprites == 0)
        return ;
    
    // Allocate memory for sprite orders and distances
    sprite_distance = cube->entities->sprite_distance;
    sprite_order = cube->entities->sprite_order;
    
    // Calculate distances for each sprite
    for (int i = 0; i < num_sprites; i++) {
        sprite_order[i] = i;
        sprite_distance[i] = sprite_enemy_get_distance(enemies[i], player);
    }
    
    // Sort sprites based on distance (furthest first)
    sprites_sort(sprite_order, sprite_distance, num_sprites);
    
    // Process each sprite
    for (int i = 0; i < num_sprites; i++) {
        // Get current sprite
        t_enemy *sprite = enemies[sprite_order[i]];
        
        // Translate sprite position relative to camera
        double sprite_x = sprite->base->current_location.x - player->base->current_location.x;
        double sprite_y = sprite->base->current_location.y - player->base->current_location.y;
        
        // Transform sprite with the inverse camera matrix
        double inv_det = 1.0 / (player->camera.dir_x * player->dir.dir_y - 
                               player->dir.dir_x * player->camera.dir_y);
        
        double transform_x = inv_det * (player->dir.dir_y * sprite_x - 
                                      player->dir.dir_x * sprite_y);
        double transform_y = inv_det * (-player->camera.dir_y * sprite_x + 
                                      player->camera.dir_x * sprite_y);
        
        // Calculate sprite's screen position
        int sprite_screen_x = (int)((WINDOW_WIDTH / 2) * (1 + transform_x / transform_y));
        
        // Calculate sprite dimensions on screen
        int sprite_height = abs((int)(WINDOW_HEIGHT / transform_y));
        int sprite_width = sprite_height; // Assuming square sprites
        
        // Calculate sprite drawing boundaries
        int draw_start_y = -sprite_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_start_y < 0) draw_start_y = 0;
        
        int draw_end_y = sprite_height / 2 + WINDOW_HEIGHT / 2;
        if (draw_end_y >= WINDOW_HEIGHT) draw_end_y = WINDOW_HEIGHT - 1;
        
        int draw_start_x = -sprite_width / 2 + sprite_screen_x;
        if (draw_start_x < 0) draw_start_x = 0;
        
        int draw_end_x = sprite_width / 2 + sprite_screen_x;
        if (draw_end_x >= WINDOW_WIDTH) draw_end_x = WINDOW_WIDTH - 1;
        
        // Draw the sprite
        for (int stripe = draw_start_x; stripe < draw_end_x; stripe++) {
            // Skip if sprite is behind a wall
            if (transform_y <= 0 || stripe < 0 || stripe >= WINDOW_WIDTH || 
                transform_y >= entities->buffer_z[stripe])
                continue;
                
            // Calculate texture x-coordinate
            int tex_x = (int)(256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) * 
                            TEXTURE_SIZE / sprite_width) / 256;
            
            // Draw vertical stripe of the sprite
            for (int y = draw_start_y; y < draw_end_y; y++) {
                // Calculate texture y-coordinate
                int d = (y * 256) - (WINDOW_HEIGHT * 128) + (sprite_height * 128);
                int tex_y = ((d * TEXTURE_SIZE) / sprite_height) / 256;
                
                // Get color from sprite texture
                unsigned int color = 0;
				tex = sprite->base->controller->current->frames_ptr->frames[sprite->base->controller->current->frame];
                if (tex)
                    color = tex[TEXTURE_SIZE * tex_y + tex_x];
                // Draw pixel if it's not transparent (assuming 0 is transparent)
                if ((color & 0x00FFFFFF) != 0)
                    draw_my_mlx_pixel_put(cube->mlx_handler->mlx_img, stripe, y, color);
            }
        }
    }

}