#include <cube_graphics.h>
#include <cube.h>
#include <cube_controls.h>
#include <math.h>

static void handle_left_right(t_cube *cube, double move_speed)
{
	double new_x;
	double new_y;
	t_point *pos;
	t_dda_vector mod_dir_vect;

	pos = &cube->player->location;
	if (cube->keys->a)
	{
		mod_dir_vect.dir_x = cos(cube->player->angle + M_PI / 2);
		mod_dir_vect.dir_y = sin(cube->player->angle + M_PI / 2);
		new_x = pos->x - mod_dir_vect.dir_x * move_speed;
		new_y = pos->y - mod_dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}
	else if (cube->keys->d)
	{
		mod_dir_vect.dir_x = cos(cube->player->angle + M_PI / 2);
		mod_dir_vect.dir_y = sin(cube->player->angle + M_PI / 2);
		new_x = pos->x + mod_dir_vect.dir_x * move_speed;
		new_y = pos->y + mod_dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}
}

static void handle_for_back(t_cube *cube, t_draw_scene_data scene, double move_speed)
{
	double new_x;
	double new_y;
	t_point *pos;

	pos = &cube->player->location;
	if (cube->keys->w)
	{
		new_x = pos->x + scene.dir_vect.dir_x * move_speed;
		new_y = pos->y + scene.dir_vect.dir_y * move_speed;
		if (cube->map->tiles[(int)new_y][(int)new_x].c == '0')
		{
			pos->x = new_x;
			pos->y = new_y;
		}
	}
	else if (cube->keys->s)
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

static void handle_rotation(t_cube *cube, double r_speed)
{
	if (cube->keys->right)
		cube->player->angle += r_speed;
	if (cube->keys->left)
		cube->player->angle -= r_speed;
}

void mov_handler(t_cube *cube)
{
	double m_speed;
	double r_speed;
	t_draw_scene_data scene;

	scene = draw_prep_scene(cube);
	cube->frame_time = (cube->time - cube->old_time) / 1000.0;
	m_speed = cube->frame_time * 5.0;
	r_speed = cube->frame_time * 3.0;
	if (cube->keys->w || cube->keys->s)
		handle_for_back(cube, scene, m_speed);
	if (cube->keys->a || cube->keys->d)
		handle_left_right(cube, m_speed);
	if (cube->keys->left || cube->keys->right)
		handle_rotation(cube, r_speed);
	if (cube->keys->escape)
		close_cube(cube);
}

int con_key_press(int key_code, t_cube *cube)
{
	t_keys *keys;

	keys = cube->keys;
	if (key_code == KEY_W)
		keys->w = 1;
	else if (key_code == KEY_A)
		keys->a = 1;
	else if (key_code == KEY_S)
		keys->s = 1;
	else if (key_code == KEY_D)
		keys->d = 1;
	else if (key_code == KEY_ARROW_LEFT)
		keys->left = 1;
	else if (key_code == KEY_ARROW_RIGHT)
		keys->right = 1;
	else if (key_code == KEY_ESC)
		keys->escape = 1;
	return (0);
}

int con_key_release(int key_code, t_cube *cube)
{
	t_keys *keys;

	keys = cube->keys;
	if (key_code == KEY_W)
		keys->w = 0;
	else if (key_code == KEY_A)
		keys->a = 0;
	else if (key_code == KEY_S)
		keys->s = 0;
	else if (key_code == KEY_D)
		keys->d = 0;
	else if (key_code == KEY_ARROW_LEFT)
		keys->left = 0;
	else if (key_code == KEY_ARROW_RIGHT)
		keys->right = 0;
	return (0);
}