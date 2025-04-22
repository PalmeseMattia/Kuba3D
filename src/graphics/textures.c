#include <cube_graphics.h>
#include <cube.h>
#include <libft.h>

// First, let's update the t_tex structure to properly allocate memory for texels
void tex_single_load(const char *path, t_tex *tex, t_cube *cube)
{
    t_image_data    img;
    int             img_width;
    int             img_height;
    int             y;
    int             x;

    if (!tex)
        return;
    
    // Allocate memory for texels if not already allocated
    if (!tex->texels)
        tex->texels = malloc(sizeof(unsigned int) * TEXTURE_SIZE * TEXTURE_SIZE);
    if (!tex->texels)
        return;
        
    img.img = mlx_xpm_file_to_image(cube->mlx, (char *)path, &img_width, &img_height);
    if (!img.img)
    {
		free(tex->texels);
        ft_printf("Failed to load texture: %s\n", path);
        return;
    }
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);
    if (!img.addr)
        return;
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

            // Convert to the format used in your buffer
            tex->texels[dst_pos] = (r << 16) | (g << 8) | b;
        }
    }
	// Log the texels values for debugging
	ft_printf("Texels for texture %s:\n", path);
	for (y = 0; y < TEXTURE_SIZE; y++)
	{
		for (x = 0; x < TEXTURE_SIZE; x++)
		{
			int pos = y * TEXTURE_SIZE + x;
			ft_printf("%08X ", tex->texels[pos]);
		}
		ft_printf("\n");
	}
    // Free the temporary image data
    mlx_destroy_image(cube->mlx, img.img);
    ft_printf("Successfully loaded texture: %s\n", path);
}

void tex_load(const char **paths, t_cube *cube)
{
    int i;

    i = -1;
    cube->textures = malloc(sizeof(t_tex *) * TEXTURES_COUNT);
    if (!cube->textures)
        return;
        
    // Initialize each texture
    while (++i < TEXTURES_COUNT)
    {
        cube->textures[i] = malloc(sizeof(t_tex));
        if (!cube->textures[i])
            return;
        cube->textures[i]->texels = NULL;
        tex_single_load(paths[i], cube->textures[i], cube);
    }
    ft_printf("Textures initialized properly\n");
}

void free_cube_textures(t_cube *cube)
{
    int i;

    i = -1;
    if (cube && cube->textures)
    {
        while (++i < TEXTURES_COUNT)
        {
            if (cube->textures[i])
            {
                if (cube->textures[i]->texels)
                    free(cube->textures[i]->texels);
                free(cube->textures[i]);
            }
        }
        free(cube->textures);
    }
}