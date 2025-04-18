#include <cube.h>
#include <utils.h>
#include <math.h>
#include <cube_graphics.h>
#include <libft.h>

int	main( void )
{
	t_cube			cube;

	initialize_game(&cube);
	print_map(*cube.map);
	mlx_hook(cube.mlx_win, 2, 1L << 0, con_key_press, &cube);
	mlx_hook(cube.mlx_win, 3, 1L << 1, con_key_release, &cube);
	mlx_loop_hook(cube.mlx, game_loop_hook, &cube);
	mlx_loop(cube.mlx);
	return (EXIT_SUCCESS);
}