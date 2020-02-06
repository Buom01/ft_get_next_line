/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: badam <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/04 17:06:05 by badam             #+#    #+#             */
/*   Updated: 2020/02/05 22:18:15 by badam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int		buff_strip_used(size_t len, char *buffer)
{
	size_t	i;
	int		same_line;

	same_line = 1;
	i = 0;
	if (len < BUFFER_SIZE && (buffer[len] == '\n' || buffer[len] == '\r'))
	{
		len++;
		same_line = 0;
	}
	while (i + len < BUFFER_SIZE)
	{
		buffer[i] = buffer[i + len];
		i++;
	}
	while (i < BUFFER_SIZE)
		buffer[i++] = '\0';
	return (same_line);
}

static size_t	buff_curnt_line_len(char *buffer)
{
	size_t	len;

	len = 0;
	while (len < BUFFER_SIZE && *buffer && *buffer != '\r' && *buffer != '\n')
	{
		len++;
		buffer++;
	}
	return (len);
}

static int		line_joinbuff(char **line, char *buffer, size_t len)
{
	size_t	newlen;
	char	*newline;
	char	*newlinecpy;
	char	*linecpy;

	newlen = ft_strlen(*line) + len + 1;
	if (!(newline = malloc(newlen * sizeof(char))))
	{
		free(*line);
		return (0);
	}
	newlinecpy = newline;
	linecpy = *line;
	while (*linecpy)
		*(newlinecpy++) = *(linecpy++);
	while (len && len--)
		*(newlinecpy++) = *(buffer++);
	*newlinecpy = '\0';
	free(*line);
	*line = newline;
	return (1);
}

static int		joinline(char **line, char *buffer)
{
	size_t	bufflen;

	bufflen = buff_curnt_line_len(buffer);
	if (bufflen)
		if (!line_joinbuff(line, buffer, bufflen))
			return (-1);
	return (buff_strip_used(bufflen, buffer));
}

int				get_next_line(int fd, char **line)
{
	static char	buffer[BUFFER_SIZE];
	int			prefill;
	int			eof;
	int			join_state;

	if (fd < 0 || !line || !(*line = malloc(sizeof(char))))
		return (-1);
	**line = '\0';
	eof = 1;
	join_state = 1;
	prefill = BUFFER_SIZE && *buffer != 0;
	while (prefill || (join_state && (eof = read(fd, buffer, BUFFER_SIZE)) > 0))
	{
		if ((join_state = joinline(line, buffer)) == -1)
			return (-1);
		prefill = 0;
	}
	if (eof == -1)
		free(*line);
	return (eof > 0 ? 1 : eof);
}
