#ifndef CUBE_MLX_HANDLER_H
# define CUBE_MLX_HANDLER_H

# define WINDOW_WIDTH 1280
# define WINDOW_HEIGHT 720
# define WINDOW_TITLE "Kuba3D"

# define WINDOW_CENTER_X (WINDOW_WIDTH / 2)
# define WINDOW_CENTER_Y (WINDOW_HEIGHT /  2)

# define FOV .66

typedef struct	s_image_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_image_data;

typedef struct	s_mlx_handler
{
	void			*mlx;
	void			*mlx_win;
	t_image_data	*mlx_img;
}	t_mlx_handler;

t_mlx_handler	*mlx_mlx_handler_init( void );
void			mlx_mlx_handler_free(t_mlx_handler *mlx_handler);

#endif