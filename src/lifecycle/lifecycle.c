#include <cube_lifecycle.h>
#include <cube_runtime.h>
#include <cube_drawing.h>
#include <cube_input_handler.h>
#include <cube.h>
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


int game_loop_hook(t_cube *cube)
{
	if (!cube->runtime_handler->running)
	{
		cube_cube_free(cube);
		return (0);
	}
	cube->runtime_handler->old_time = cube->runtime_handler->time;
	cube->runtime_handler->time = framerate_get_ticks();
	printf("Loop at: %ld\n", cube->runtime_handler->time);
	draw_scene(cube);
	mov_handler(cube);
	return (0);
}