#ifndef CUBE_MOCK_H
# define CUBE_MOCK_H

# define MAP_SIZE 24
# define PLAYER_START_X 6
# define PLAYER_START_Y (MAP_SIZE - 5)

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define PLAYER_VIEWING_ANGLE M_PI;

# include <parsing.h>

t_color		load_mock_color( void );
t_settings	*load_mock_data( void );
t_map		*load_mock_map( void );
t_player	*load_mock_player( void );
int			is_within_map_bounds(t_point pt, t_map map);

#endif