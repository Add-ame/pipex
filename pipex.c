#include "pipex.h"

/*
Check if infile exist or not
if it exist open it in read mode
*/

void	free_pt(char **pt)
{
	int	i;

	i = 0;
	while (pt && pt[i])
		free(pt[i++]);
	free(pt);
}

char	*check_cmd_env(char *argv, char **env)
{
	int		i, j, finded;
	char	**pt;
	char	*cmd;
	char	*tmp;

	if (!argv || !ft_strlen(argv) || !c_w(argv, ' '))
		return (NULL);
	i = 0;
	finded = 0;
	while (++i >= 0 && env[i])
		if (strncmp("PATH=", env[i], 5) == 0 && ++finded == 1)
			break;
	pt = ft_split(env[i] + 5, ':');
	finded = 0;
	i = 0;
	while (pt[i])
	{
		tmp = pt[i];
		pt[i] = ft_strjoin(tmp, "/");
		free(tmp);
		cmd = ft_strjoin(pt[i], argv);
		if (access(cmd, F_OK | X_OK) == 0)
		{
			finded = 1;
			break;
		}
		free(cmd);
		i++;
	}
	if (!finded)
		return (free_pt(pt), NULL);
	return (pt[i]);
}

void	put_error(char *s)
{
	write(2, "command not found: ", 19);
	while (*s)
		write(2, s++, 1);
	write(2, "\n", 1);
}

/**
 * 1- open infile
 * 2- check if cmd exist in env path
 * 3- execute the cmd 
 * 4- Wait for 1 cmd to finish
 * 5- fork for cmd_2
 * 6- do same
 * 7- Wait for cmd_2 to finish
 * 8- fork for cmd_2
 * 9- do same...
 */

int     main(int ac, char **av, char **env)
{
	int		pid_1, pid_2;
	int		i, j;
	int		fd[2], fd1, fd2;
	int		wait_status, status_code;
	char	*cmd_1, *cmd_2, **argv;

	pipe(fd);
	fd1 = open(av[1], O_RDONLY);
	pid_1 = fork();
	if (pid_1 == 0)
	{
		dup2(fd1, 0);
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		argv = ft_split(av[2], ' ');
		cmd_1 = check_cmd_env(argv[0], env);
		if (!cmd_1)
		{
			put_error(argv[0]);
			free_pt(argv);
			exit(127);
		}
		cmd_1 = ft_strjoin(cmd_1, argv[0]);
		execve(cmd_1, argv, NULL);
	}
	else
		close(fd1);

	fd2 = open(av[4], O_CREAT | O_WRONLY, 0777);
	pid_2 = fork();
	if (pid_2 == 0)
	{
		dup2(fd[0], 0);
		dup2(fd2, 1);
		close(fd[0]);
		close(fd[1]);
		argv = ft_split(av[3], ' ');
		cmd_2 = check_cmd_env(argv[0], env);
		if (!cmd_2)
		{
			put_error(argv[0]);
			free_pt(argv);
			exit(127);
		}
		cmd_2 = ft_strjoin(cmd_2, argv[0]);
		execve(cmd_2, argv, NULL);
	}
	else
		close(fd1);

	close(fd[0]);
	close(fd[1]);
	waitpid(pid_1, NULL, 0);
	wait(&wait_status);
	status_code = WEXITSTATUS(wait_status);
	exit(status_code);
	return (0);
}
