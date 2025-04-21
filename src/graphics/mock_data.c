/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnenczak <jnenczak@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:15:33 by jnenczak          #+#    #+#             */
/*   Updated: 2025/04/18 21:08:03 by jnenczak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cube_mock.h>
#include <stdlib.h>

t_color	load_mock_color( void )
{
	t_color	ret;

	ret.blue = 0;
	ret.green = 255;
	ret.red = 0;
	return (ret);
}


t_settings	*load_mock_data( void )
{
	t_settings	*ret;

	ret = malloc(sizeof(t_settings));
	if (!ret)
		return (NULL);
	ret->ceiling = load_mock_color();
	ret->floor = load_mock_color();
	ret->textures[0].path = "assets/textures/birch_planks.png";
	ret->textures[1].path = "assets/textures/birch_planks.png";
	ret->textures[2].path = "assets/textures/birch_planks.png";
	ret->textures[3].path = "assets/textures/birch_planks.png";
	return (ret);
}

t_map	*load_mock_map( void )
{
	t_map	*map;
	int		y, x;

	int worldMap[MAP_SIZE][MAP_SIZE] = {
		{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
		{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
		{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
		{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
		{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
		{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
		{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
		{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
		{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
		{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
		{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
		{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
		{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
		{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
		{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
		{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
		{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
		{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
		{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
		{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
		{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
		{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
	};

	map = malloc(sizeof(t_map));
	if (!map)
		return (NULL);
	y = -1;
	while (++y < MAP_SIZE)
	{
		x = -1;
		while (++x < MAP_SIZE)
		{
			map->tiles[y][x].y = y;
			map->tiles[y][x].x = x;
			
			map->tiles[y][x].c = (worldMap[y][x] == 0) ? '0' : '1';
		}
	}
	map->tiles[PLAYER_START_Y][PLAYER_START_X].c = 'E';
	return (map);
}

t_player	*load_mock_player( void )
{
	t_player	*ret;

	ret = malloc(sizeof(t_player));
	if (!ret)
		return (NULL);
	ret->location.x = PLAYER_START_X;
	ret->location.y = PLAYER_START_Y;
	ret->angle = PLAYER_VIEWING_ANGLE; // slightly rotated from straight left

	// TODO: Remove artificial offset once controls work
	ret->angle -= 0.2;
	return(ret);
}

// TODO: Implement a real check to see for maps that are not squares
int	is_within_map_bounds(t_point pt, t_map map)
{
	(void)map;
	return ((pt.x >= 0 && pt.x < MAP_SIZE) && (pt.y >= 0 && pt.x < MAP_SIZE));
}