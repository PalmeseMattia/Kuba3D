#include <cube_graphics.h>
#include <cube.h>
#include <libft.h>

void	tex_single_load(const char *path, t_tex *tex, t_cube *cube)
{
	t_image_data	img;
	int 			img_width;
	int 			img_height;
	int 			y;
	int 			x;

	if (!tex)
		return ;
	img.img = mlx_xpm_file_to_image(cube->mlx, (char *)path, &img_width, &img_height);
	if (!img.img)
	{
		ft_printf("Failed to load texture: %s\n", path);
		return ;
	}
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
								 &img.endian);
	if (!img.addr)
		return ;
	y = -1;
	while (++y < TEXTURE_SIZE)
	{
		x = -1;
		while (++x < TEXTURE_SIZE)
		{
			int src_pos = y * img.line_length + x * (img.bits_per_pixel / 8);
			int dst_pos = y * TEXTURE_SIZE + x;

			// Extract RGB values (MLX format is usually BGRA or ARGB)
			unsigned char b = img.addr[src_pos];
			unsigned char g = img.addr[src_pos + 1];
			unsigned char r = img.addr[src_pos + 2];

			// Convert to the format used in your buffer (depends on your specific implementation)
			tex->texels[dst_pos] = (r << 16) | (g << 8) | b;
		}
	}
	ft_printf("Successfully loaded texture: %s\n", path);
}

void	tex_load(const char **paths, t_cube *cube)
{
	int i;

	i = -1;
	cube->textures = malloc(sizeof(t_tex) * TEXTURES_COUNT);
	if (!cube->textures)
		return;
	while (++i < TEXTURES_COUNT)
		tex_single_load(paths[i], cube->textures[i], cube);
	ft_printf("Textures initialized properly\n");
}

void	tex_get(t_cube *cube, t_dda dda)
{
	t_map	*map;

	map = cube->map;

	int texNum = map->tiles[dda.map_x][dda.map_y].c - '1';

	double wallX;
	// TODO: verify if vertical / horizontal
	if (dda.hit_type == VERTICAL) wallX = dda.pos_y + dda.perp_wall_dist * dda.ray_dir_y;
	else           wallX = dda.pos_x + dda.perp_wall_dist * dda.ray_dir_x;
	wallX -= floor((wallX));

	//x coordinate on the texture
	int texX = (int)wallX * (double)TEXTURE_SIZE;
	if(dda.hit_type == VERTICAL && dda.ray_dir_x > 0) texX = TEXTURE_SIZE - texX - 1;
	if(dda.hit_type == HORIZONTAL && dda.ray_dir_y < 0) texX = TEXTURE_SIZE - texX - 1;

}