#include <cube_input_handler.h>
#include <cube_map.h>
#include <cube.h>
#include <cube_entities.h>
#include <cube_runtime.h>
#include <math.h>
#include <stdio.h>

static void handle_left_right(t_cube *cube, double move_speed)
{
    double					new_x;
    double					new_y;
    t_player				*player;
    t_input_handler_keys	*keys;
    t_vector 				side_dir;

    player = cube->entities->player;
    keys = cube->input_handler->keys;    
    side_dir.dir_x = -player->dir.dir_y;
    side_dir.dir_y = player->dir.dir_x;
    if (keys->a)
    {
        new_x = player->x + side_dir.dir_x * move_speed;
        new_y = player->y + side_dir.dir_y * move_speed;
        if (map_is_walkable(cube->map->tiles[(int)new_y][(int)new_x]))
        {
            player->x = new_x;
            player->y = new_y;
        }
    }
    else if (keys->d)
    {
        new_x = player->x - side_dir.dir_x * move_speed;
        new_y = player->y - side_dir.dir_y * move_speed;
        if (map_is_walkable(cube->map->tiles[(int)new_y][(int)new_x]))
        {
            player->x = new_x;
            player->y = new_y;
        }
    }
}

static void handle_for_back(t_cube *cube, double move_speed)
{
	double					new_x;
	double					new_y;
	t_player				*player;
	t_input_handler_keys	*keys;

	keys = cube->input_handler->keys;
	player = cube->entities->player;
	if (keys->w)
	{
		new_x = player->x + player->dir.dir_x * move_speed;
		new_y = player->y + player->dir.dir_y * move_speed;
		if (map_is_walkable(cube->map->tiles[(int)new_y][(int)new_x]))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	else if (keys->s)
	{
		new_x = player->x - player->dir.dir_x * move_speed;
		new_y = player->y - player->dir.dir_y * move_speed;
		if (map_is_walkable(cube->map->tiles[(int)new_y][(int)new_x]))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
}

static void handle_rotation(t_cube *cube, double r_speed)
{
    t_player	*player;
    t_vector	dir_vect;
    t_vector	camera_vect;

    player = cube->entities->player;    
    if (cube->input_handler->keys->right)
    {
        dir_vect.dir_x = player->dir.dir_x * cos(-r_speed) - player->dir.dir_y * sin(-r_speed);
        dir_vect.dir_y = player->dir.dir_x * sin(-r_speed) + player->dir.dir_y * cos(-r_speed);
        camera_vect.dir_x = player->camera.dir_x * cos(-r_speed) - player->camera.dir_y * sin(-r_speed);
        camera_vect.dir_y = player->camera.dir_x * sin(-r_speed) + player->camera.dir_y * cos(-r_speed);
        player->dir = dir_vect;
        player->camera = camera_vect;
    }
    else if (cube->input_handler->keys->left)
    {
        dir_vect.dir_x = player->dir.dir_x * cos(r_speed) - player->dir.dir_y * sin(r_speed);
        dir_vect.dir_y = player->dir.dir_x * sin(r_speed) + player->dir.dir_y * cos(r_speed);
        camera_vect.dir_x = player->camera.dir_x * cos(r_speed) - player->camera.dir_y * sin(r_speed);
        camera_vect.dir_y = player->camera.dir_x * sin(r_speed) + player->camera.dir_y * cos(r_speed);
        player->dir = dir_vect;
        player->camera = camera_vect;
    }
}

int input_handler_key_press(int key_code, t_cube *cube)
{
	t_input_handler_keys	*keys;

	keys = cube->input_handler->keys;
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

int input_handler_key_release(int key_code, t_cube *cube)
{
	t_input_handler_keys	*keys;

	keys = cube->input_handler->keys;
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

void	mov_handler(t_cube *cube)
{
	double					m_speed;
	double					r_speed;
	t_runtime_handler		*rh;
	t_input_handler_keys	*keys;

	rh = cube->runtime_handler;
	keys = cube->input_handler->keys;
	rh->frametime = (rh->time - rh->old_time) / 1000.0;
	m_speed = rh->frametime * 5.0;
	r_speed = rh->frametime * 3.0;
	if (keys->w || keys->s)
	{
		handle_for_back(cube, m_speed);
		printf("Forward/Backward movement handled. W: %d, S: %d\n", keys->w, keys->s);
	}
	if (keys->a || keys->d)
	{
		handle_left_right(cube, m_speed);
		printf("Left/Right movement handled. A: %d, D: %d\n", keys->a, keys->d);
	}
	if (keys->left || keys->right)
	{
		handle_rotation(cube, r_speed);
		printf("Rotation handled. Left: %d, Right: %d\n", keys->left, keys->right);
	}
	if (keys->escape)
		cube->runtime_handler->running = FALSE;
}