#include "parse.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int parse_texture(t_texture *tex, char *line)
{
	line = line + 2;
	if (!isspace(*line))
	{
		printf("unexpected character `%c'", *line);
		return (-1);
	}
	if (*tex->path != '\0')
	{
		printf("duplicate texture `%c%c'", line[-2], line[-1]);
		return (-1);
	}
	line = skip_space(line);
	return (parse_path(tex->path, line));
}

int parse_textures(t_textures *textures, char *line)
{
	if (strncmp("NO", line, 2) == 0)
	{
		return (parse_texture(&textures->no, line));
	}
	else if (strncmp("SO", line, 2) == 0)
	{
		return (parse_texture(&textures->so, line));
	}
	else if (strncmp("WE", line, 2) == 0)
	{
		return (parse_texture(&textures->we, line));
	}
	else if (strncmp("EA", line, 2) == 0)
	{
		return (parse_texture(&textures->ea, line));
	}
	return (1);
}
