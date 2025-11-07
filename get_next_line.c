/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <thiferre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 10:22:51 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/07 13:13:57 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	char		*line;
	static char	*text;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	text = ft_read_file(fd, text);
	if (!text)
		return (NULL);
	line = ft_find_line(text);
	text = ft_leftover(text);
	return (line);
}

char	*ft_read_file(int fd, char *text)
{
	int		bytes_read;
	char	*buff;

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

char	*ft_find_line(char *text)
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
		i++;
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
	free (text);
	return (left);
}
