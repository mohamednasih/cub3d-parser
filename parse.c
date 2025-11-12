#include <linux/limits.h>
#include <stdio.h>
char *get_next_line(int fd);
typedef struct s_texture
{
	void *img;
	int *addr;
	int width;
	int height;
	int bpp;
	int line_len;
	int endian;
	char path[PATH_MAX];
	int check_duplicate;
} t_texture;

typedef struct s_textures
{
	t_texture no;
	t_texture so;
	t_texture we;
	t_texture ea;
} t_textures;

typedef struct s_colors
{
	int floor;
	int ceiling;
} t_colors;

typedef struct s_cub3d
{
    void 	*mlx_ptr;
    void 	*win;
	void 	*img;
	char	*data_addr;
	int		bits_per_pixel;
	int		size_line;
	int 	endianne;
    int 	player_x;
    int 	player_y;
	double	rotating_angle;
	int		width;
	int 	nbr_rays;
	int		rows;
	int		columns;
	char	**map;
	char		*av;
	t_textures  tex;
    t_colors    colors;
} t_cub3d;

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char	*skip_space(char *line)
{
	while (*line && *line == ' ')
	{
		line++;
	}
	return (line);
}
int valid_file(const char *path, const char *extention)
{
	const char *ext;
	size_t len_path;
	size_t len_extention;

	len_path = strlen(path);
	len_extention = strlen(extention);
	ext = strrchr(path, '.');
	if (ext == NULL || strcmp(ext, extention))
	{
		printf("invalid file name `%s'", path);
		return (-1);
	}
	if ((len_path - (*path == '/')) <= len_extention)
	{
		printf("invalid file name `%s'", path);
		return (-1);
	}
	return (0);
}

int parse_path(char *path, char *line)
{
	char *start;
	char *cursor;
	size_t len;

	cursor = line;
	len = 0;
	while (*cursor && *cursor != '\n')
	{
		path[len++] = *cursor;
		cursor++;
	}
	if (*cursor != '\n')
	{
		return (-1);
	}
	path[len] = 0;
	if (valid_file(path,".xpm") == -1)
	{
		return (-1);
	}
	return (0);
}

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

int parse_colors_or_texture(t_textures *textures, t_colors *colors, char *line)
{
	int status;

	status = parse_textures(textures, line);
	if (status != 1)
		return (status);
	return (parse_colors(colors, line));
}

int	parse(t_cub3d *cub, int fd)
{
	char	*line;
	int		parsing_status;

	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		if (*line == '\n')
			continue;
		parsing_status = parse_colors_or_texture(&cub->tex, &cub->colors, line);
		if (parsing_status == -1)
		{
			printf(":line:%s", line);
			return (-1);
		}
		if (parsing_status == 0)
			continue;
		break ;
	}
}

int	main()
{
	t_textures textures;
	t_colors	colors;
	t_cub3d		cub;

	cub = (t_cub3d){0};
	cub.colors.ceiling = -1;
	cub.colors.floor = -1;
	cub.tex.no.path[0] = '\0';
	cub.tex.so.path[0] = '\0';
	cub.tex.we.path[0] = '\0';
	cub.tex.ea.path[0] = '\0';
	
	int r = parse(&cub, 0);
	printf("%d\n", r);
}
