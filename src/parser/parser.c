#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

enum e_cardinal {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

typedef struct s_texture
{
	char	*path;
}	t_texture;

typedef struct s_color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}	t_color;

typedef struct s_settings
{
	t_texture	textures[4];
	t_color		floor;
	t_color		ceiling;
}	t_settings;

t_settings	*new_settings()
{
	t_settings	*settings;

	settings = (t_settings *)ft_calloc(sizeof(t_settings), 1);
	for (int i = 0; i < 4; i++)
	{
		settings->textures[i].path = NULL;
	}
	return (settings);
}

int	ft_strisdigit(char *str)
{
	if (str == NULL)
		return 0;
	while (str != NULL && *str)
	{
		if (!ft_isdigit(*str))
			return 0;
		str++;
	}
	return 1;
}

void	parse_color(char *line, t_color *color)
{
	char	**splitted;
	char	**color_components;
	int		color_value;

	splitted = ft_split(line, ' ');
	if (ft_strarr_len(splitted) != 2)
		printf("Error in line \"%s\".\n", line);
	else
	{
		color_components = ft_split(splitted[1], ',');
		if (ft_strarr_len(color_components) != 3)
			printf("Not enough colors in line \"%s\".\n", line);
		else
		{
			for (int i = 0; i < 3; i++)
			{
				if (!ft_strisdigit(color_components[i]))
				{
					printf("Wrong color: \"%s\".\n", color_components[i]);
					return;
				}
			}
			for (int i = 0; i < 3; i++)
			{
				color_value = ft_atoi(color_components[i]);
				if (color_value < 0 || color_value > 255)
				{
					printf("Error in line \"%s\":", line);
					printf("Color values should be between 0 and 255\n");
					return ;
				}
			}
			color->red = ft_atoi(color_components[0]);
			color->green = ft_atoi(color_components[1]);
			color->blue = ft_atoi(color_components[2]);
		}
	}
	//TODO: free splitted
}

void	parse_texture(char *line, t_texture *texture)
{
	char	**splitted;
	int		size;

	splitted = ft_split(line, ' ');
	size = ft_strarr_len(splitted);
	if (size != 2)
	{
		printf("I don\'t know WTF are you trying to do. ");
		printf("I will use some default settings here! \n");
	} else {
		if (texture -> path != NULL)
		{
			printf("Overriding texture\n"); 
			free(texture->path);
		}
		texture->path = ft_calloc(ft_strlen(splitted[1]), sizeof(char));
		ft_strncpy(texture->path, splitted[1], ft_strlen(splitted[1]) - 1);
		printf("Added texture: %s\n", texture->path);
	}
	//TODO: free splitted
}

void parse_settings(char *file_path)
{
	int			fd;
	char		*line;
	char		*cardinals[4] = {"NO", "SO", "EA", "WE"};
	char		**splitted;
	t_settings	*settings;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
	{
		perror("Something went wrong wile opening the config file!");
		exit(EXIT_FAILURE);
	}
	settings = new_settings();
	while ((line = get_next_line(fd))) {
		for (int i = 0; i < 4; i++) {
			if ((ft_strncmp(line, cardinals[i], 2)) == 0)
			{
				parse_texture(line, &settings->textures[i]);
				break;
			}
		}
		if (ft_strncmp(line, "F ", 2) == 0) {
			printf("Found floor color\n");
			parse_color(line, &settings->floor);
		}
		else if (ft_strncmp(line, "C ", 2) == 0) {
			printf("Found Ceiling color\n");
			parse_color(line, &settings->ceiling);
		}
	}
}

int main()
{
	parse_settings("test.cub");
}
