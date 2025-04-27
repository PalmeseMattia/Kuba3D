#ifndef CUBE_RUNTIME_H
# define CUBE_RUNTIME_H

# include <stdlib.h>
# include <utils.h>

typedef struct	s_runtime_handler
{
	size_t	time;
	size_t	old_time;
	t_bool	running;
	double	frametime;
	double	frames_per_second;
}	t_runtime_handler;

t_runtime_handler	*runtime_runtime_handler_init( void );
void				runtime_runtime_handler_free(t_runtime_handler *runtime_handler);

#endif