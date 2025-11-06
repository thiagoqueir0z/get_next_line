#include <fcntl.h>
#include "get_next_line.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    char *line;
    int fd[FOPEN_MAX];
    int n_fds;
    int n;
    int i;

    if (argc < 2)
	{
		printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
	}
	// open fds
    n_fds = (argc - 1);
	for (i = 0; i < n_fds; ++i) 
	{
		if ((fd[i] = open(argv[(i + 1)], O_RDONLY)) == -1)
		{
			printf("Error opening file: %s\n", argv[i + 1]);
			return 1;
		}
	}
	// test get_next_line
    printf("Testing get_next_line w BUFFER_SIZE = %d\n\n", BUFFER_SIZE);
	n = 1;
	line = NULL;
    for (i = 0; i < n_fds; ++i)
	{
        while ((line = get_next_line(fd[i])) != NULL)
		{
			printf("%d:\t%s", n, line);
            free(line);
            ++n;
            if (++i >= n_fds)
                i = 0; // Wrap around to the first file descriptor
        }
    }
	// close fds
	for (i = 0; i < n_fds; ++i)
		close(fd[i]);
    return 0;
}