/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <thiferre@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 10:22:51 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/07 14:47:10 by thiferre         ###   ########.fr       */
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
	if (!line)
		return (NULL);
	return (line);
}

char	*ft_read_file(int fd, char *text)
{
	int		bytes_read;
	char	*buff;

	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	bytes_read = 1;
	while (bytes_read != 0 && (!text || !ft_strchr(text, '\n')))
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buff), free(text), NULL);
		if (bytes_read == 0)
			break ;
		buff[bytes_read] = '\0';
		text = ft_strjoin(text, buff);
		if (!text)
			return (free(buff), NULL);
	}
	free(buff);
	if (!text || !*text)
		return (free(text), NULL);
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

	if (!text)
		return (NULL);
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
	if (!left[0])
		return (free(left), NULL);
	return (left);
}
