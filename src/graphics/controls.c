#include <cube_graphics.h>
#include <cube.h>
#include <math.h>

void	quit_game(t_cube *cube)
{
	cube->running = 0;
	// mlx_terminate(cube->mlx);
	// free_game(cube);
}

static void	handle_left_right(t_cube *cube, double move_speed)
{
	double	new_x;
	double	new_y;
	t_point	*pos;
	t_vect	mod_dir_vect;

	pos = &cube->player->location;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_A))
	{
		mod_dir_vect.dir_x = cos(cube->player->angle + M_PI /  2);
		mod_dir_vect.dir_y = sin(cube->player->angle + M_PI /  2);
		new_x = pos->x - mod_dir_vect.dir_x * move_speed;
		new_y = pos->y - mod_dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}
	else if (mlx_is_key_down(cube->mlx, MLX_KEY_D))
	{
		mod_dir_vect.dir_x = cos(cube->player->angle + M_PI /  2);
		mod_dir_vect.dir_y = sin(cube->player->angle + M_PI /  2);
		new_x = pos->x + mod_dir_vect.dir_x * move_speed;
		new_y = pos->y + mod_dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}	
}

static void	handle_for_back(t_cube *cube, t_scene_setup scene, double move_speed)
{
	double	new_x;
	double	new_y;
	t_point	*pos;

	pos = &cube->player->location;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_W))
	{
		new_x = pos->x + scene.dir_vect.dir_x * move_speed;
		new_y = pos->y + scene.dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}
	else if (mlx_is_key_down(cube->mlx, MLX_KEY_S))
	{
		new_x = pos->x - scene.dir_vect.dir_x * move_speed;
		new_y = pos->y - scene.dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}
}

static void	handle_rotation(t_cube *cube, double r_speed)
{
	if (mlx_is_key_down(cube->mlx, MLX_KEY_RIGHT))
		cube->player->angle += r_speed;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_LEFT))
		cube->player->angle -= r_speed;
}

void	mov_handle_keypress(t_cube *cube, t_scene_setup scene)
{
	double	frameTime;
	double	m_speed;
	double	r_speed;

	cube->old_time = cube->time;
	cube->time = framerate_get_ticks();
	frameTime = (cube->time - cube->old_time) / 1000.0;
	m_speed = frameTime * 5.0;
	r_speed = frameTime * 3.0;
	if (mlx_is_key_down(cube->mlx, MLX_KEY_W) || mlx_is_key_down(cube->mlx, MLX_KEY_S))
		handle_for_back(cube, scene, m_speed);
	if (mlx_is_key_down(cube->mlx, MLX_KEY_A) || mlx_is_key_down(cube->mlx, MLX_KEY_D))
		handle_left_right(cube, m_speed);
	if (mlx_is_key_down(cube->mlx, MLX_KEY_RIGHT) || mlx_is_key_down(cube->mlx, MLX_KEY_LEFT))
		handle_rotation(cube, r_speed);
	if (mlx_is_key_down(cube->mlx, MLX_KEY_ESCAPE))
		quit_game(cube);
}