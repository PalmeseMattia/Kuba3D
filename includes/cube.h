#ifndef CUBE_H
# define CUBE_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <mlx.h>
# include <cube_mock.h>
# include <cube_controls.h>

# define FOV .66
# define TILE_SIZE 1
# define WINDOW_TITLE "Cub3D"
# define WINDOW_RESIZE FALSE

typedef struct	s_image_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_image_data;

typedef struct	s_tex	t_tex;

typedef struct	s_cube
{
	t_settings		*settings;
	t_map			*map;
	t_player		*player;

	void			*mlx;
	void			*mlx_win;
	t_image_data	*mlx_img;
	
	size_t			time;
	size_t			old_time;
	t_bool			running;
	
	double			frame_time;

	t_keys			*keys;

	t_tex			**textures;
}	t_cube;

#endif