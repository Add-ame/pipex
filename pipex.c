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
	char	**argv;

	fd[0] = open(av[1], O_RDONLY);
	if (fd[0] == -1)
	{
		perror("Error");
		return (1);
	}

	id = fork();
	if (id == 0)
	{
		argv = ft_split(av[2], ' ');
		dup2(fd[0], 0);
		if (execve("/home/maddame/Desktop/pipex/script", argv, NULL) == -1)
		{
			perror(NULL);
			return (1);
		}
	}
	else
	{
		wait(NULL);
	}
	return (0);
}
