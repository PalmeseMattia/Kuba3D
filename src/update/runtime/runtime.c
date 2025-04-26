#include <cube_runtime.h>

t_runtime_handler	*runtime_runtime_handler_init( void )
{
	t_runtime_handler	*runtime_handler;

	runtime_handler = malloc(sizeof(t_runtime_handler));
	if (!runtime_handler)
		return (NULL);
	runtime_handler->frames_per_second = 0;
	runtime_handler->frametime = 0;
	runtime_handler->time = 0;
	runtime_handler->old_time = 0;
	runtime_handler->running = TRUE;
	return (runtime_handler);
}

void	runtime_runtime_handler_free(t_runtime_handler *runtime_handler)
{
	if (!runtime_handler)
		return ;
	free(runtime_handler);
	runtime_handler = NULL;
}