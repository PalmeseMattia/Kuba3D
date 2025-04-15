#include <parsing.h>
#include <cube_mock.h>
#include <stdio.h>

void	print_map(t_map map)
{
	int	x, y;

	y = -1;
	printf("--- PRINTING MAP ---\n");
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
			printf("%c", map.tiles[y][x].c);
		printf("\n");
	}
	printf("--- / PRINTING MAP ---\n");
}