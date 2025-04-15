#ifndef CUBE_MOCK_H
# define CUBE_MOCK_H

# define MAP_SIZE 16
# define PLAYER_START_X MAP_SIZE / 2
# define PLAYER_START_Y MAP_SIZE / 2

# include <parsing.h>

t_color		load_mock_color( void );
t_settings	*load_mock_data( void );
t_map		*load_mock_map( void );
t_player	*load_mock_player( void );

#endif