#include <cube_mlx_handler.h>
#include <stdlib.h>
#include <mlx.h>
#include <cube_settings.h>
#include <utils.h>
#include <libft.h>
#include <cube_mlx_handler.h>
#include <cube_settings_animated_sprites.h>

t_tex_config	*settings_tex_config_init(const char **paths, t_mlx_handler *mlx_handler)
{
	t_tex_config	*tex_config;

	tex_config = malloc(sizeof(t_tex_config));
	if (!tex_config)
		return (NULL);
	tex_config->textures = malloc(sizeof(size_t *) * (TEXTURE_TYPES_COUNT));
	tex_config->enemy_frames = settings_enemy_frames_init(mlx_handler);
	tex_config->exit_frames = settings_exit_frames_init(mlx_handler);
	if (!tex_config->textures)
	{
		safe_free(tex_config);
		return (NULL);
	}
	settings_tex_config_set_tex_all(tex_config, mlx_handler, paths);
	return (tex_config);
}

static t_image_data *settings_tex_get_image(const char *path, t_mlx_handler *mlx_handler)
{
    t_image_data	*img;
    int				img_width;
    int				img_height;

	img = malloc(sizeof(t_image_data));
	if (!img)
		return (NULL);
    img->img = mlx_xpm_file_to_image(mlx_handler->mlx, (char *)path, &img_width, &img_height);
    if (!img->img)
	{
		free(img);
		return (NULL);
	}
    img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length,
                                &img->endian);
	if (!img->addr)
	{
		free(img);
		return (NULL);
	}
    return (img);
}

size_t	*settings_tex_get_texels(const char *path, t_mlx_handler *mlx_handler)
{
	t_image_data			*img;
	size_t					*texels;
	int						y;
	int						x;
	t_single_tex_load_data	tex_data;
	

	img = settings_tex_get_image(path, mlx_handler);
    if (!img)
        return (NULL);
    texels = malloc(sizeof(size_t) * (TEXTURE_SIZE * TEXTURE_SIZE));
    if (!texels)
    {
        mlx_destroy_image(mlx_handler->mlx, img);
        free(img);
        return (NULL);
    }

	tex_data.img = img;
	y = -1;
	while (++y < TEXTURE_SIZE)
	{
		x = -1;
		while (++x < TEXTURE_SIZE)
		{
			tex_data.src_pos = y * tex_data.img->line_length + x * (tex_data.img->bits_per_pixel / 8);
			tex_data.dst_pos = y * TEXTURE_SIZE + x;
			tex_data.color.b = tex_data.img->addr[tex_data.src_pos];
			tex_data.color.g = tex_data.img->addr[tex_data.src_pos + 1];
			tex_data.color.r = tex_data.img->addr[tex_data.src_pos + 2];
			texels[tex_data.dst_pos] = (tex_data.color.r << 16) | (tex_data.color.g << 8) | tex_data.color.b;
		}
	}
	mlx_destroy_image(mlx_handler->mlx, tex_data.img->img);
	free(tex_data.img);
	ft_printf("Successfully loaded texture: %s\n", path);
	return (texels);	
}

size_t	*settings_tex_config_set_tex(t_tex_config *config, t_tex_type tex_type,
	t_mlx_handler *mlx_handler, const char *path)
{
	if (!config)
		return (NULL);
	config->textures[tex_type] = settings_tex_get_texels(path + 3, mlx_handler);
	return (config->textures[tex_type]);
}

t_tex_type	settings_tex_get_tex_type(const char *path)
{
	if (!path)
		return (TEX_TYPE_NORTH);
	if (!ft_strncmp(path, "NO", 2))
		return (TEX_TYPE_NORTH);
	if (!ft_strncmp(path, "SO", 2))
		return (TEX_TYPE_SOUTH);
	if (!ft_strncmp(path, "WE", 2))
		return (TEX_TYPE_WEST);
	if (!ft_strncmp(path, "EA", 2))
		return (TEX_TYPE_EAST);
	if (!ft_strncmp(path, "XX", 2))
		return (TEX_TYPE_ENEMY);
	if (!ft_strncmp(path, "FL", 2))
		return (TEX_TYPE_FLOOR);
	if (!ft_strncmp(path, "CE", 2))
		return (TEX_TYPE_CEILING);
	if (!ft_strncmp(path, "EX", 2))
		return (TEX_TYPE_EXIT);
	if (!ft_strncmp(path, "ST", 2))
		return (TEX_TYPE_SPAWN);
	if (!ft_strncmp(path, "KE", 2))
		return (TEX_TYPE_KEYCARD);
	return (TEX_TYPE_NORTH);
}

void	settings_tex_config_set_tex_all(t_tex_config *config, t_mlx_handler *mlx_handler, const char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = -1;
	while (++i < TEXTURE_TYPES_COUNT)
		config->textures[i] = NULL;
	i = -1;
	while (++i < TEXTURE_TYPES_COUNT)
	{
		settings_tex_config_set_tex(config, 
			settings_tex_get_tex_type((char *)paths[i]), mlx_handler, (char *)paths[i]);
	}
}

// Add to settings_tex_config_free in textures.c
void settings_tex_config_free(t_tex_config *tex_config)
{
    int i;

    if (!tex_config)
        return;
    
    // Free individual textures
    if (tex_config->textures)
    {
        i = -1;
        while (++i < TEXTURE_TYPES_COUNT)
            safe_free(tex_config->textures[i]);
        safe_free(tex_config->textures);
    }
    
    // Free animation frames for enemy
    if (tex_config->enemy_frames)
    {
        if (tex_config->enemy_frames->frames)
        {
            for (i = 0; i < tex_config->enemy_frames->frames_count; i++)
                safe_free(tex_config->enemy_frames->frames[i]);
            safe_free(tex_config->enemy_frames->frames);
        }
        safe_free(tex_config->enemy_frames);
    }
    
    // Free animation frames for exit
    if (tex_config->exit_frames)
    {
        if (tex_config->exit_frames->frames)
        {
            for (i = 0; i < tex_config->exit_frames->frames_count; i++)
                safe_free(tex_config->exit_frames->frames[i]);
            safe_free(tex_config->exit_frames->frames);
        }
        safe_free(tex_config->exit_frames);
    }
    
    safe_free(tex_config);
}