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
	mlx_key_hook(cube.mlx_win, con_key_hook, &cube);
	mlx_loop_hook(cube.mlx, game_loop_hook, &cube);
	mlx_loop(cube.mlx);
	return (EXIT_SUCCESS);
}