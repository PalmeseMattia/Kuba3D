#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <libft.h>
#include <parsing.h>

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
			if ((ft_strncmp(line, cardinals[i], 2)) == 0) {
				printf("Found cardinal point %s\n", cardinals[i]);
				splitted = ft_split(line, ' ');
				int size = ft_strarr_len(splitted);
				if (size != 2)
				{
					printf("I don\'t know WTF are you trying to do. ");
					printf("I will use some default settings here! \n");
				} else {
					if (settings->textures[i].path != NULL)
					{
						printf("Overriding texture for %s\n", cardinals[i]);
						free(settings->textures[i].path);
					}
					settings->textures[i].path = ft_calloc(ft_strlen(splitted[1]), sizeof(char));
					ft_strncpy(settings->textures[i].path, splitted[1], ft_strlen(splitted[1]) - 1);
					printf("Added texture: %s\n", settings->textures[i].path);
				}
				//TODO: free splitted
			}
		}
		if (ft_strncmp(line, "F ", 2) == 0) {
			printf("Found floor color\n");
		}
		else if (ft_strncmp(line, "C ", 2) == 0) {
			printf("Found Ceiling color\n");
		}
	}
}
