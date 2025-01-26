#include "pipex.h"

/*
Check if infile exist or not
if it exist open it in read mode
*/
/*
char	*check_cmd_env(char *argv, char **env)
{
	int		i;
	int		j;
	char	**pt;
	char	*path;
	char	cmp[6];
	int		finded;

	i = 0;
	finded = 0;
	cmp[6] = "PATH=";
	while (env[i])
	{
		j = 0;
		while (env[i][j] && cmp[j] && j < 5 && env[i][j] == cmp[j])
			j++;
		if (cmp[j] == '\0')
		{
			finded = 1;
			break;
		}
		i++;
	}

	if (!finded)
		return (NULL);
	pt = ft_split(env[i] + 4, ':'); // 4 | 5
	
}*/

int     main(int ac, char **av, char **env)
{
	int		fd[2];
	int		pid1;
	int		pid2;
	int		fd1;
	int		fd2;
	int		i;
	int		j;
	char	**argv;
	char	*cmd1;
	char	*cmd2;
	int		wait_status;
	int		status_code;


	fd1 = open(av[1], O_RDONLY);
	pipe(fd);// fd[0] fd[1]
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(fd1, 0);
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		argv = ft_split(av[2], ' ');
		// cmd1 = check_cmd_env(argv[0], env);
		cmd1 = ft_strjoin("/usr/bin/", argv[0]);
		if (access(cmd1, F_OK | X_OK) == -1)
		{
			write(2, "Command not found\n", 18);
			exit(127);
		}
		if (execve(cmd1, argv, NULL) == -1)
		{
			perror(NULL);
			exit (errno); // temporary
		}
		// the output of execve should be in fd[1] and next cmd should take that as input
		// dup(fd[1], 1);
	}
	else
		close(fd1);

	fd2 = open(av[4], O_CREAT | O_WRONLY, 0777);
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], 0);
		dup2(fd2, 1);
		close(fd[0]);
		close(fd[1]);
		argv = ft_split(av[3], ' ');
		// cmd1 = check_cmd_env(argv[0], env);
		cmd2 = ft_strjoin("/usr/bin/", argv[0]);
		if (access(cmd2, F_OK | X_OK) == -1)
		{
			write(2, "Command not found\n", 18);
			exit(127);
		}
		if (execve(cmd2, argv, NULL) == -1)
		{
			perror(NULL);
			exit (errno); // temporary
		}
		// the output of execve should be in fd[1] and next cmd should take that as input
		// dup(fd[1], 1);
	}
	else
		close(fd2);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status_code, 0);
	status_code = WEXITSTATUS(wait_status);
	exit(status_code);
	return (0);
}
