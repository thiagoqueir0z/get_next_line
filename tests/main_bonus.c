/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thiferre <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 11:35:56 by thiferre          #+#    #+#             */
/*   Updated: 2025/11/07 11:35:59 by thiferre         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <fcntl.h>  // open()
#include <stdio.h>  // printf()
#include <stdlib.h> // free()
#include <unistd.h> // close()

int main(void)
{
	int		fd1;
	int		fd2;
	char	*line1;
	char	*line2;

	// 1️⃣ Open two different files
	fd1 = open("test_file.txt", O_RDONLY);
	fd2 = open("test_boundary.txt", O_RDONLY);

	if (fd1 < 0 || fd2 < 0)
	{
		printf("❌ Error opening files!\n");
		return (1);
	}

	printf("🟢 Reading alternately between two files:\n\n");

	// 2️⃣ Read alternately between the two files
	while (1)
	{
		line1 = get_next_line(fd1);
		line2 = get_next_line(fd2);

		if (!line1 && !line2)
			break;

		if (line1)
		{
			printf("📘 [FILE 1] %s", line1);
			free(line1);
		}
		if (line2)
		{
			printf("📗 [FILE 2] %s", line2);
			free(line2);
		}
	}

	// 3️⃣ Close the files
	close(fd1);
	close(fd2);

	printf("\n✅ Reading completed!\n");
	return (0);
}

