#ifndef CUBE_INPUT_HANDLER_H
# define CUBE_INPUT_HANDLER_H

# define KEY_ESC 65307
# define KEY_ARROW_LEFT 65361
# define KEY_ARROW_RIGHT 65363
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_E 101 // TODO: Verify

typedef struct s_input_handler_keys
{
    int	w;
    int	a;
    int	s;
    int	d;
    int	left;
    int	right;
    int	escape;

	// Action Keys
	int	e;
}	t_input_handler_keys;

typedef struct s_input_handler
{
	t_input_handler_keys	*keys;
}	t_input_handler;

typedef struct s_cube	t_cube;

t_input_handler	*input_handler_init( void );
void			input_handler_free(t_input_handler *input_handler);
int				input_handler_key_release(int key_code, t_cube *cube);
int				input_handler_key_press(int key_code, t_cube *cube);
void			mov_handler(t_cube *cube);

#endif