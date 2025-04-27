#include <X11/X.h>
#include <cube.h>
#include <stdlib.h>
#include <cube_runtime.h>
#include <stdio.h>
#include <cube_mock.h>
#include <cube_settings_textures.h>
#include <cube_mlx_handler.h>
#include <cube_lifecycle.h>
#include <cube_input_handler.h>
#include <mlx.h>

char *map[] = {
	"111111111111111111111111",
	"100000000000000010000001",
	"101000000000000000000001",
	"101000000000000000000001",
	"101000000000000010000001",
	"101000011111111110111111",
	"101000010101010110001111",
	"101000010000000010000001",
	"101000000000000000000001",
	"101000010000000010000001",
	"100000010000000010001111",
	"100000011110111111111111",
	"111111111110111111111111",
	"100000000000000000000001",
	"111111011110111111111111",
	"111111011110111111111111",
	"100000000011011000001001",
	"100000000000110010001001",
	"100000000011011000011011",
	"101010000011000010000001",
	"100100000011011000011011",
	"10101000001101101E001001",
	"1Q0000000000000000000001",
	"111111111111111111111111"
};

const char *paths[] = {
	"NO ./assets/textures/biohazard/xpm/wall_north.xpm",
	"SO ./assets/textures/biohazard/xpm/wall_south.xpm",
	"EA ./assets/textures/biohazard/xpm/wall_east.xpm",
	"WE ./assets/textures/biohazard/xpm/wall_west.xpm",
	"FL ./assets/textures/biohazard/xpm/floor.xpm",
	"CE ./assets/textures/biohazard/xpm/ceiling.xpm",
	"XX ./assets/textures/biohazard/enemies/enemy_1.xpm",
};

int	main( void )
{
	t_cube	*cube;

	cube = cube_cube_init((char **)map, MAP_SIZE, MAP_SIZE, paths);
	mlx_hook(cube->mlx_handler->mlx_win, 2, 1L << 0, input_handler_key_press, cube);
	mlx_hook(cube->mlx_handler->mlx_win, 3, 1L << 1, input_handler_key_release, cube);
	mlx_hook(cube->mlx_handler->mlx_win, DestroyNotify,
		StructureNotifyMask, &on_destroy, &cube);
	mlx_loop_hook(cube->mlx_handler->mlx, game_loop_hook, cube);
	mlx_loop(cube->mlx_handler->mlx);
	return (EXIT_SUCCESS);
}