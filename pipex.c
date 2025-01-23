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
	// if (id == 0)
	// 	sleep(1);

	if (id == 0)
	{
		printf("child::::Current ID: %d, Parent ID: %d\n", getpid(), getppid());
	}
	else
		printf("Current ID: %d, Parent ID: %d\n", getpid(), getppid());

	int		res;
	res = wait(NULL);
	if (res == -1)
		printf("No childre to wait for\n");
	else
		printf("%d finished execution\n", res);
	return (0);
}
