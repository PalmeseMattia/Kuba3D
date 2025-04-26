#include <cube_mlx_handler.h>
#include <stdlib.h>
#include <mlx.h>
#include <utils.h>

t_mlx_handler	*mlx_mlx_handler_init( void )
{
	t_mlx_handler	*mlx_handler;

	mlx_handler = malloc(sizeof(t_mlx_handler));
	if (!mlx_handler)
		return (NULL);
	mlx_handler->mlx = mlx_init();
	mlx_handler->mlx_win = mlx_new_window(mlx_handler->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Kuba3D");
	mlx_handler->mlx_img = mlx_new_image(mlx_handler->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	return (mlx_handler);
}

void	mlx_mlx_handler_free(t_mlx_handler *mlx_handler)
{
	mlx_destroy_image(mlx_handler->mlx, mlx_handler->mlx_img);
	mlx_destroy_window(mlx_handler->mlx, mlx_handler->mlx_win);
	mlx_destroy_display(mlx_handler->mlx);
	safe_free(mlx_handler);
}