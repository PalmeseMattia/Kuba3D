#include <cube_lifecycle.h>
#include <cube_runtime.h>
#include <cube_drawing.h>
#include <cube_input_handler.h>
#include <cube_animations.h>
#include <cube_settings_animated_sprites.h>
#include <cube.h>
#include <cube_entities.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

int		on_destroy(t_cube *cube)
{
	input_handler_key_press(KEY_ESC, cube);
	return (0);
}

static double framerate_get_ticks()
{
    struct timeval	tv;
	
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000.0) + (tv.tv_usec / 1000.0);
}

static void	animate_sprites(t_cube *cube)
{
	int						i;
	int						frame;
	t_enemy					*enemy;
	t_animation_controller	*controller;

	static double	last_time = 0;
	double			current_time;

	current_time = framerate_get_ticks();
	if (current_time - last_time >= 250) // 250 ms = 4 times a second
	{
		i = -1;
		while (cube->entities->enemies[++i])
		{
			enemy = cube->entities->enemies[i];
			controller = enemy->base->controller;
			if (controller->playing)
			{
				frame = ++controller->current->frame;
				if (frame >= controller->current->frames_ptr->frames_count - 1 && !controller->repeat)
				{
					controller->playing = FALSE;
					controller->current = controller->idle;
					controller->current->frame = 0;
				}
				controller->current->frame = frame % 
					(controller->current->frames_ptr->frames_count);
			}
			controller = cube->entities->exit->base->controller;
			if (controller->playing) 
			{
				frame = ++controller->current->frame;
				if (frame >= controller->current->frames_ptr->frames_count && !controller->repeat)
				{
					controller->playing = FALSE;
					--controller->current->frame;
				}
				else 
				{
					controller->current->frame = frame % 
						(controller->current->frames_ptr->frames_count);
				}
			}
		}
		last_time = current_time;
	}
}

int game_loop_hook(t_cube *cube)
{
	if (!cube->runtime_handler->running)
	{
		cube_cube_free(cube);
		return (0);
	}
	cube->runtime_handler->old_time = cube->runtime_handler->time;
	cube->runtime_handler->time = framerate_get_ticks();
	animate_sprites(cube);
	draw_scene(cube);
	mov_handler(cube);
	return (0);
}