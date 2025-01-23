#include "pipex.h"

/*
Check if infile exist or not
if it exist open it in read mode
*/
int     main(int ac, char **av)
{
	int		fd[2];
	int		id;
	int		i;
	int		j;

	if (ac != 3)
		return (0);
	fd[0] = open(av[1], O_RDONLY);
	if (fd[0] == -1)
	{
		perror("Error");
		return (1);
	}

	id = fork();
	if (id == 0)
	{
		char	*argv[] = {av[2], NULL};
		dup2(fd[0], 0);
		execve(av[2], argv, NULL);
	}
	else
	{
		wait(NULL);
	}
	return (0);
}
