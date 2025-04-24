#ifndef CUBE_TEXTURES_H
# define CUBE_TEXTURES_H

# define TEXTURES_COUNT 5
# define TEXELS_COUNT (WINDOW_HEIGHT * WINDOW_WIDTH)
# define TEXTURE_SIZE 512
# define FLOOR_TEXTURE 4

typedef struct	s_tex_color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}	t_tex_color;
typedef struct	s_tex
{
	unsigned int	*texels;
}	t_tex;

# include "cube.h"

typedef struct	s_single_tex_load_data
{
	t_image_data	img;
	t_tex_color		color;
	int				src_pos;
	int				dst_pos;
}	t_single_tex_load_data;
#endif 