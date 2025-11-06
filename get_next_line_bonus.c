/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <thiferre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:34:02 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/06 16:31:05 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	*text[4096];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	if (!text[fd])
	{
		text[fd] = malloc(1);
		if (!text[fd])
			return (NULL);
		text[fd][0] = '\0';
	}
	text[fd] = ft_read_file(fd, text[fd]);
	line = ft_find_line(text[fd]);
	text[fd] = ft_leftover(text[fd]);
	return (line);
}

char	*ft_read_file(int fd, char *text)
{
	char	*buff;
	int		bytes_read;

	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	bytes_read = 1;
	while (bytes_read != 0 && !ft_strchr(text, '\n'))
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buff), free(text), NULL);
		if (bytes_read == 0)
			break ;
		buff[bytes_read] = '\0';
		text = ft_strjoin(text, buff);
	}
	free(buff);
	return (text);
}

char	*ft_find_line(char	*text)
{
	int		i;
	char	*line;

	i = 0;
	if (!text[i])
		return (NULL);
	while (text[i] && text[i] != '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 2));
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
		++i;
	if (!text[i])
		return (free(text), NULL);
	left = (char *)malloc(sizeof(char) * (ft_strlen(text) - i + 1));
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
