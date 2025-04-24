#include <cube.h>
#include <cube_graphics.h>
#include <cube_textures.h>
#include <libft.h>

int		on_destroy(t_cube *cube)
{
	con_key_press(KEY_ESC, cube);
	return (0);
}

static void	free_cube_textures(t_cube *cube)
{
	int	i;

	i = -1;
	if (cube && cube->textures)
	{
		while (++i < TEXTURES_COUNT)
			free(cube->textures[i]);
		free(cube->textures);
	}
}

void close_cube(t_cube *cube)
{
    printf("Closing cube\n");
    if (!cube)
        return;
    cube->running = FALSE;
    if (cube->settings)
        free(cube->settings);
    if (cube->map)
        free(cube->map);
    if (cube->player)
        free(cube->player);
    if (cube->mlx_img)
    {
        if (cube->mlx_img->img)
            mlx_destroy_image(cube->mlx, cube->mlx_img->img);
        free(cube->mlx_img);  // Add this line to free the structure itself
    }
    if (cube->mlx_win)
        mlx_destroy_window(cube->mlx, cube->mlx_win);
    if (cube->mlx)
        mlx_destroy_display(cube->mlx);
    if (cube->textures)
     free_cube_textures(cube);
    if (cube->keys)
        free(cube->keys);
}

static void close_error(t_cube *cube)
{
	ft_printf("Error initializing MLX\n");
	close_cube(cube);
	exit(EXIT_FAILURE);
}

static void	close_success(t_cube *cube)
{
	ft_printf("Gently Closing Cube\n");
	close_cube(cube);
	exit(EXIT_SUCCESS);
}

static void initialize_mlx(t_cube *cube)
{
    cube->mlx = NULL;
    cube->mlx_win = NULL;
    
    // Allocate image data structure on the heap
    cube->mlx_img = malloc(sizeof(t_image_data));
    if (!cube->mlx_img)
    {
		close_error(cube);
        return;
    }
    
    // Initialize the structure fields
    cube->mlx_img->img = NULL;
    cube->mlx_img->addr = NULL;
    cube->mlx_img->bits_per_pixel = 0;
    cube->mlx_img->line_length = 0;
    cube->mlx_img->endian = 0;
    
    cube->mlx = mlx_init();
    if (!cube->mlx)
    {
        close_error(cube);
        return;
    }
    
    cube->mlx_win = mlx_new_window(cube->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    if (!cube->mlx_win)
    {
        close_error(cube);
        return;
    }
}

void	initialize_keys(t_cube *cube)
{
	t_keys	*keys;

	keys = malloc(sizeof(t_keys));
	if (!keys)
		return ;
	keys->a = 0;
	keys->d = 0;
	keys->s = 0;
	keys->w = 0;
	keys->left = 0;
	keys->right = 0;
	keys->escape = 0;
	cube->keys = keys;
}

void	initialize_game(t_cube *cube)
{
	cube->settings = load_mock_data();
	cube->map = load_mock_map();
	cube->player = load_mock_player();
	cube->time = 0;
	cube->old_time = 0;
	cube->running = TRUE;
	cube->frame_time = 0;
	cube->textures = NULL;
	initialize_keys(cube);
	initialize_mlx(cube);
	load_mock_textures(cube);
}

int game_loop_hook(t_cube *cube)
{
	if (!cube->running)
	{
		close_success(cube);
		return (0);
	}
	cube->old_time = cube->time;
	cube->time = framerate_get_ticks();
	draw_scene(cube);
	mov_handler(cube);
	return (0);
}