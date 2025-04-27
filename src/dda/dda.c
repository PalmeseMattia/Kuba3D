#include <cube_dda.h>
#include <cube_mlx_handler.h>
#include <cube.h>
#include <cube_entities.h>
#include <stdlib.h>
#include <cube_map.h>
#include <math.h>

void dda_setup(t_cube *cube, double camera_x)
{
    t_player *player;
    t_dda_data *dda;

    player = cube->entities->player;
    dda = cube->dda_data;
    
    // Copy player's direction and camera vectors
    dda->dir_vect = player->dir;
    dda->camera_dir = player->camera;
    
    // Calculate ray direction
    dda->ray_dir.dir_x = dda->dir_vect.dir_x + dda->camera_dir.dir_x * camera_x;
    dda->ray_dir.dir_y = dda->dir_vect.dir_y + dda->camera_dir.dir_y * camera_x;
    
    dda->pos_x = player->x;
    dda->pos_y = player->y;
    dda->map_x = (int)dda->pos_x;
    dda->map_y = (int)dda->pos_y;
    
    // Calculate delta distances
    if (dda->ray_dir.dir_x == 0)
        dda->delta_dist_x = 1e30;
    else
        dda->delta_dist_x = fabs(1 / dda->ray_dir.dir_x);
    if (dda->ray_dir.dir_y == 0)
        dda->delta_dist_y = 1e30;
    else
        dda->delta_dist_y = fabs(1 / dda->ray_dir.dir_y);
}

void	dda_perform(t_cube *cube)
{
	t_dda_data	*dda;
	int			hit;

	hit = 0;
	dda = cube->dda_data;
	while (hit == 0)
	{
		if (dda->side_dist_x < dda->side_dist_y)
		{
			dda->side_dist_x += dda->delta_dist_x;
			dda->map_x += dda->step_x;
			dda->hit_type = HORIZONTAL;
		}
		else
		{
			dda->side_dist_y += dda->delta_dist_y;
			dda->map_y += dda->step_y;
			dda->hit_type = VERTICAL;
		}

		// Check if ray has hit a wall
		t_point pt;
		pt.x = dda->map_x;
		pt.y = dda->map_y;
		if (!map_is_within_bounds(pt.x, pt.y, *cube->map))
		{
			hit = 1;
			break;
		}
		if (cube->map->tiles[dda->map_y][dda->map_x].c == '1')
			hit = 1;
	}
}

void	dda_set_step_and_initial_side_dist(t_cube *cube)
{
	t_dda_data	*dda;
	
	dda = cube->dda_data;
	if (dda->ray_dir.dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (dda->pos_x - dda->map_x) * dda->delta_dist_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_dist_x = (dda->map_x + 1.0 - dda->pos_x) * dda->delta_dist_x;
	}
	if (dda->ray_dir.dir_y < 0)
	{
		dda->step_y = -1;
		dda->side_dist_y = (dda->pos_y - dda->map_y) * dda->delta_dist_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_dist_y = (dda->map_y + 1.0 - dda->pos_y) * dda->delta_dist_y;
	}
}

t_dda_data	*dda_init()
{
	t_dda_data	*dda_data;

	dda_data = malloc(sizeof(t_dda_data));
	if (!dda_data)
		return (NULL);
	return (dda_data);
}

void	dda_free(t_dda_data *dda)
{
	safe_free(dda);
}