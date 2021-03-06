/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvanden- <jvanden-@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 16:24:15 by jvanden-          #+#    #+#             */
/*   Updated: 2021/01/27 16:27:46 by jvanden-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cube.h"

void				get_nsew(t_cube *cube)
{
	int y;
	int x;

	y = 0;
	cube->cam.pos.x = -1;
	cube->cam.pos.y = -1;
	while (cube->map.map[y])
	{
		x = 0;
		while (cube->map.map[y][x])
		{
			if (isinstr("NSEW", cube->map.map[y][x]))
				get_pos(cube, x, y, cube->map.map[y][x]);
			x++;
		}
		y++;
	}
	if (cube->cam.pos.x == -1)
		handle_error_parsing("You must add a starting point in map", cube);
}

static void			get_map_line(t_cube *cube, int *mapline)
{
	int		fd;
	char	*filedata;
	int		check;

	if ((fd = open(cube->filename, O_RDONLY)) == -1)
		handle_error_parsing("Can't read .cub file", cube);
	while ((check = get_next_line(fd, &filedata)))
	{
		if (check == -1)
			handle_error_parsing("get next line alloc", cube);
		if (is_map_line(filedata) && ft_strlen(filedata))
			(*mapline)++;
		if (check == 2 || (!(is_map_line(filedata)) && *mapline))
		{
			free(filedata);
			break ;
		}
		free(filedata);
	}
	close(fd);
}

static void			malloc_map(t_cube *cube)
{
	int mapline;

	mapline = 0;
	get_map_line(cube, &mapline);
	if (!(cube->map.map = malloc(sizeof(char*) * mapline + 1)))
		handle_error_parsing("Can't malloc map", cube);
	cube->map.map[mapline] = NULL;
}

void				parsing_map(t_cube *cube)
{
	int		fd;
	char	*filedata;
	int		i;
	int		check;

	i = 0;
	malloc_map(cube);
	if ((fd = open(cube->filename, O_RDONLY)) == -1)
		handle_error_parsing("Can't read .cub file", cube);
	while ((check = get_next_line(fd, &filedata)))
	{
		if (check == -1)
			handle_error_parsing("get next line alloc", cube);
		if (is_map_line(filedata) && ft_strlen(filedata))
			cube->map.map[i++] = filedata;
		if (!(is_map_line(filedata)) && i)
			free(filedata);
		if (check == 2 || (!(is_map_line(filedata)) && i))
			break ;
	}
	close(fd);
	get_nsew(cube);
}
