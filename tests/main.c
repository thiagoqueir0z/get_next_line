/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 09:48:06 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/07 11:20:14 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>   // for open()
#include <stdio.h>   // for printf()
#include <stdlib.h>  // for free()
#include <unistd.h>  // for close()

int main(void)
{
	int		fd;	  // file descriptor (number representing the opened file)
	char	*line;	// pointer that will receive each line

	// 1️⃣ Open the file "test_file.txt" in read-only mode
	fd = open("test_file.txt", O_RDONLY);
	if (fd < 0)
	{
		printf("Error opening file!\n");
		return (1);
	}

	// 2️⃣ Read the file line by line
	while (1)
	{
		line = get_next_line(fd); // read the next line
		if (line == NULL)	  // if there are no more lines (EOF)
			break;

		printf("%s", line);	  // print the read line
		free(line);		  // free the line memory
	}

	// 3️⃣ Close the file and exit
	close(fd);
	return (0);
}

