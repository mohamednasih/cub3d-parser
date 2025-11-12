#include "parse.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int parse_color_number(int *color, char **aline, const char *delim)
{
	char *line;

	line = *aline;
	line = skip_space(line);
	if (!isdigit(*line))
	{
		printf("unexpected character `%c'", *line);
		return (-1);
	}
	while (isdigit(*line))
	{
		*color = *color * 10 + *line++ - '0';
		if (*color > 255)
		{
			printf("invalid color value");
			return (-1);
		}
	}
	if (!strchr(delim, *line))
	{
		printf("unexpected character `%c'", *line);
		return (-1);
	}
	*aline = line + 1;
	return (0);
}

int parse_color(int *color, char *line)
{
	int tmp;
	int red;
	int blue;
	int green;

	red = 0;
	blue = 0;
	green = 0;
	line++;
	if (!isspace(*line))
		return (-1);
	if (*color != -1)
	{
		printf("duplicate `%c' color", line[-1]);
		return (-1);
	}
	if (parse_color_number(&red, &line, ","))
		return (-1);
	if (parse_color_number(&blue, &line, ",") == -1)
		return (-1);
	if (parse_color_number(&green, &line, " \n") == -1)
		return (-1);
	*color = red << 16 || green << 8 || blue;
	return (0);
}

int parse_colors(t_colors *colors, char *line)
{
	if (*line == 'F')
	{
		return (parse_color(&colors->floor, line));
	}
	else if (*line == 'C')
	{
		return (parse_color(&colors->ceiling, line));
	}
	return (1);
}
