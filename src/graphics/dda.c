#include <cube_graphics.h>
#include <cube.h>
#include <math.h>

void	dda_set_wall_height(t_dda_data *dda)
{
	double perp_wall_dist;

	if (dda->hit_type == HORIZONTAL)
		perp_wall_dist = (dda->side_dist_x - dda->delta_dist_x);
	else
		perp_wall_dist = (dda->side_dist_y - dda->delta_dist_y);
	dda->wall_height = (int)(WINDOW_HEIGHT / perp_wall_dist);
}

void	dda_init(t_dda_data *dda, t_draw_scene_data *scene_setup, double camera_x)
{
	dda->ray_dir_x = scene_setup->dir_vect.dir_x + scene_setup->camera_plane_x * camera_x;
	dda->ray_dir_y = scene_setup->dir_vect.dir_y + scene_setup->camera_plane_y * camera_x;
	dda->pos_x = scene_setup->player_pos_x;
	dda->pos_y = scene_setup->player_pos_y;
	dda->map_x = (int)dda->pos_x;
	dda->map_y = (int)dda->pos_y;
	if (dda->ray_dir_x == 0)
		dda->delta_dist_x = 1e30;
	else
		dda->delta_dist_x = fabs(1 / dda->ray_dir_x);
	if (dda->ray_dir_y == 0)
		dda->delta_dist_y = 1e30;
	else
		dda->delta_dist_y = fabs(1 / dda->ray_dir_y);
}

void	dda_perform(t_dda_data *dda, t_cube *cube)
{
	int hit;

	hit = 0;
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
		if (!is_within_map_bounds(pt, *cube->map))
		{
			hit = 1;
			break;
		}
		if (cube->map->tiles[dda->map_y][dda->map_x].c == '1')
			hit = 1;
	}
}

void	dda_set_step_and_initial_side_dist(t_dda_data *dda)
{
	if (dda->ray_dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (dda->pos_x - dda->map_x) * dda->delta_dist_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_dist_x = (dda->map_x + 1.0 - dda->pos_x) * dda->delta_dist_x;
	}
	if (dda->ray_dir_y < 0)
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