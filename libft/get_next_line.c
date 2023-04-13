/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:22:45 by cbadura           #+#    #+#             */
/*   Updated: 2023/03/23 16:34:04 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	handle_storage(char *storage, char *line);
static char	*build_line_store(int fd, int *read_bytes, char *storage);
static void	cut_after_newline(char *str);
static int	check_readbytes(char *buff, char *tmp, int read_bytes);

char	*get_next_line(int fd)
{
	static char	storage[BUFFER_SIZE + 1];
	char		*line;
	char		*tmp;
	int			read_bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	read_bytes = 1;
	line = malloc(sizeof(char) * MAX_SIZE);
	if (handle_storage(storage, line))
		return (line);
	tmp = build_line_store(fd, &read_bytes, storage);
	if (!tmp)
	{
		free(line);
		return (NULL);
	}
	ft_strcat(line, tmp);
	free(tmp);
	if (ft_strlen(line) == 0 && read_bytes == 0)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

static int	handle_storage(char *storage, char *line)
{
	char	*storage_cpy;

	ft_memset(line, '\0', MAX_SIZE - 1);
	storage_cpy = storage;
	if (storage && storage[0] == '\n')
	{
		storage_cpy++;
		if (ft_strchr_gnl(storage_cpy, '\n') != NULL)
		{
			ft_strcpy(line, storage_cpy);
			ft_memset(storage, '\0', ft_strlen(storage));
			while (*line != '\n')
				line++;
			ft_strcpy(storage, line);
			cut_after_newline(line);
			return (1);
		}
		else
			ft_strcpy(line, storage_cpy);
		ft_memset(storage, '\0', ft_strlen(storage));
	}
	return (0);
}

static char	*build_line_store(int fd, int *read_bytes, char *storage)
{
	char	*tmp;
	char	*rest;
	char	*buff;

	tmp = malloc(MAX_SIZE);
	tmp[0] = '\0';
	buff = malloc(BUFFER_SIZE + 1);
	while (*read_bytes > 0)
	{
		*read_bytes = read(fd, buff, BUFFER_SIZE);
		if (check_readbytes(buff, tmp, *read_bytes))
			return (NULL);
		buff[*read_bytes] = '\0';
		if (ft_strchr_gnl(buff, '\n'))
		{
			rest = ft_strchr_gnl(buff, '\n');
			ft_strcpy(storage, rest);
			ft_strcat(tmp, buff);
			break ;
		}
		ft_strcat(tmp, buff);
	}
	cut_after_newline(tmp);
	free(buff);
	return (tmp);
}

static void	cut_after_newline(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
			return ;
		}
		i++;
	}
}

static int	check_readbytes(char *buff, char *tmp, int read_bytes)
{
	if (read_bytes == -1)
	{
		free(buff);
		free(tmp);
		return (1);
	}
	return (0);
}
