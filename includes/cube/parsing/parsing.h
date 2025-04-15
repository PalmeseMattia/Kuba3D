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

#endif