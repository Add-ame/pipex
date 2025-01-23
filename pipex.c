#include "pipex.h"

/*
Check if infile exist or not
if it exist open it in read mode
*/
int     main(int ac, char **av)
{
	// if (ac != 5)
	// 	return (1);

	// if (access(av[1], F_OK) == 0)
	// 	printf("rah kayn");
	// else
	// 	perror(NULL);

	int		id;
	int		i;

	printf("beging hello from id: %d\n", id);
	id = fork();
	if (id == 0)
	{
		printf("this is child\n");
		printf("%d\n", id);
		printf("child pid %d\n", getpid());
		// printf("%d\n", getppid());
		// printf("The owner of this process has uid %d \n",getuid());
	}
	else
	{
		printf("this is parent\n");
		printf("%d\n", id);
	}

	return (0);
}
