/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <thiferre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:34:02 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/07 12:16:00 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	*text[4096];

	if (fd < 0 || fd >= 4096 || BUFFER_SIZE <= 0)
		return (NULL);
	text[fd] = ft_read_file(fd, text[fd]);
	if (!text[fd])
		return (NULL);
	line = ft_find_line(text[fd]);
	text[fd] = ft_leftover(text[fd]);
	return (line);
}

char	*ft_read_file(int fd, char *text)
{
	char	*buff;
	int		bytes_read;

	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (free(text), NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(text, '\n'))
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(buff), free(text), NULL);
		buff[bytes_read] = '\0';
		text = ft_strjoin(text, buff);
		if (!text)
			return (free(buff), NULL);
	}
	free(buff);
	return (text);
}

char	*ft_find_line(char *text)
{
	int		i;
	char	*line;

	if (!text || !*text)
		return (NULL);
	i = 0;
	while (text[i] && text[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (text[i] && text[i] != '\n')
	{
		line[i] = text[i];
		i++;
	}
	if (text[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*ft_leftover(char *text)
{
	int		i;
	int		j;
	char	*left;

	i = 0;
	while (text[i] && text[i] != '\n')
		i++;
	if (!text[i])
		return (free(text), NULL);
	left = malloc(ft_strlen(text) - i);
	if (!left)
		return (free(text), NULL);
	i++;
	j = 0;
	while (text[i])
		left[j++] = text[i++];
	left[j] = '\0';
	free(text);
	return (left);
}
