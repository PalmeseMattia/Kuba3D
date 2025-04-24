#include <cube_graphics.h>
#include <cube_textures.h>
#include <cube.h>
#include <libft.h>

static t_image_data tex_get_image(const char *path, t_tex *tex, t_cube *cube)
{
    t_image_data img;
    int img_width;
    int img_height;

    img.img = mlx_xpm_file_to_image(cube->mlx, (char *)path, &img_width, &img_height);
    if (!img.img)
    {
        free(tex->texels);
        tex->texels = NULL;
        ft_printf("Failed to load texture: %s\n", path);
        return (img);
    }
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                &img.endian);
    return (img);
}

void tex_single_load(const char *path, t_tex *tex, t_cube *cube)
{
    t_single_tex_load_data tex_data;
    int y;
    int x;

    if (!tex)
        return;
    if (!tex->texels)
        tex->texels = malloc(sizeof(unsigned int) * TEXTURE_SIZE * TEXTURE_SIZE);
    if (!tex->texels)
        return;
    
    tex_data.img = tex_get_image(path, tex, cube);
    if (!tex_data.img.img || !tex_data.img.addr)
        return;
    
    y = -1;
    while (++y < TEXTURE_SIZE)
    {
        x = -1;
        while (++x < TEXTURE_SIZE)
        {
            tex_data.src_pos = y * tex_data.img.line_length + x * (tex_data.img.bits_per_pixel / 8);
            tex_data.dst_pos = y * TEXTURE_SIZE + x;
            tex_data.color.b = tex_data.img.addr[tex_data.src_pos];
            tex_data.color.g = tex_data.img.addr[tex_data.src_pos + 1];
            tex_data.color.r = tex_data.img.addr[tex_data.src_pos + 2];
            tex->texels[tex_data.dst_pos] = (tex_data.color.r << 16) | (tex_data.color.g << 8) | tex_data.color.b;
        }
    }
    mlx_destroy_image(cube->mlx, tex_data.img.img);
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