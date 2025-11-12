#include "parse.h"

int parse_colors_or_texture(t_textures *textures, t_colors *colors, char *line)
{
	int status;

	status = parse_textures(textures, line);
	if (status != 1)
		return (status);
	return (parse_colors(colors, line));
}
