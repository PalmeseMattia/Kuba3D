#include <cube_graphics.h>
#include <cube.h>
#include <cube_controls.h>
#include <math.h>

void	quit_game(t_cube *cube)
{
	cube->running = 0;
	// mlx_terminate(cube->mlx);
	// free_game(cube);
}

static void	handle_left_right(t_cube *cube, double move_speed, int key_code)
{
	double	new_x;
	double	new_y;
	t_point	*pos;
	t_vect	mod_dir_vect;

	pos = &cube->player->location;
	if (key_code == KEY_A)
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
	else if (key_code == KEY_D)
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

static void	handle_for_back(t_cube *cube, t_scene_setup scene, double move_speed, int key_code)
{
	double	new_x;
	double	new_y;
	t_point	*pos;

	pos = &cube->player->location;
	if (key_code == KEY_W)
	{
		new_x = pos->x + scene.dir_vect.dir_x * move_speed;
		new_y = pos->y + scene.dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}
	else if (key_code == KEY_S)
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

static void	handle_rotation(t_cube *cube, double r_speed, int key_code)
{
	if (key_code == KEY_ARROW_RIGHT)
		cube->player->angle += r_speed;
	if (key_code == KEY_ARROW_LEFT)
		cube->player->angle -= r_speed;
}

void	mov_handle_keypress(t_cube *cube, t_scene_setup scene, int key_code)
{
	double	frameTime;
	double	m_speed;
	double	r_speed;

	if (key_code)
		printf("Key pressed: %d\n", key_code);
	frameTime = (cube->time - cube->old_time) / 1000.0;
	m_speed = frameTime * 5.0;
	r_speed = frameTime * 3.0;
	if (key_code == KEY_W || key_code == KEY_S)
		handle_for_back(cube, scene, m_speed, key_code);
	if (key_code == KEY_A || key_code == KEY_D)
		handle_left_right(cube, m_speed, key_code);
	if (key_code == KEY_ARROW_LEFT || key_code == KEY_ARROW_RIGHT)
		handle_rotation(cube, r_speed, key_code);
	if (key_code == KEY_ESC)
		quit_game(cube);
}

int	con_key_hook(int key_code, t_cube *cube)
{
	t_scene_setup	scene;

	printf("Hi dead\n");
	scene = draw_scene(cube);
	mov_handle_keypress(cube, scene, key_code);
	return (0);
}