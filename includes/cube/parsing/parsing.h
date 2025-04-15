#ifndef PARSING_H
# define PARSING_H

enum e_cardinal {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

typedef struct s_texture
{
	char	*path;
}	t_texture;

typedef struct s_color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}	t_color;

typedef struct s_settings
{
	t_texture	textures[4];
	t_color		floor;
	t_color		ceiling;
}	t_settings;

typedef struct	s_point
{
	int		x;
	int		y;
	char	c;
}	t_point;

typedef struct	s_player
{
	t_point	location;
	double	angle;
}	t_player;

typedef struct	s_map
{
	t_point	tiles[128][128];
}	t_map;

#endif