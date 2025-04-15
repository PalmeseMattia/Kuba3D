/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mock_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnenczak <jnenczak@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:15:33 by jnenczak          #+#    #+#             */
/*   Updated: 2025/04/15 21:31:34 by jnenczak         ###   ########.fr       */
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
			if (y == 0 || x == 0 || x == MAP_SIZE - 1 || y == MAP_SIZE - 1)
				map->tiles[y][x].c = 1;
			else
				map->tiles[y][x].c = 0;
		}
	}
	return (map);
}