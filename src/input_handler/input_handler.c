#include <cube_input_handler.h>
#include <utils.h>
#include <stdlib.h>

t_input_handler_keys	*input_handler_keys_init( void )
{
	t_input_handler_keys	*keys;

	keys = malloc(sizeof(t_input_handler_keys));
	if (!keys)
		return (NULL);
	keys->a = 0;
	keys->w = 0;
	keys->s = 0;
	keys->d = 0;
	keys->escape = 0;
	keys->left = 0;
	keys->right = 0;
	keys->e = 0;
	return (keys);
}

t_input_handler	*input_handler_init( void )
{
	t_input_handler	*input_handler;

	input_handler = malloc(sizeof(t_input_handler));
	if (!input_handler)
		return (NULL);
	input_handler->keys = input_handler_keys_init();
	if (!input_handler->keys)
	{
		safe_free(input_handler);
		return (NULL);
	}
	return (input_handler);
}

void	input_handler_free(t_input_handler *input_handler)
{
	safe_free(input_handler->keys);
	safe_free(input_handler);
}