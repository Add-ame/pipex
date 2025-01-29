#include "pipex.h"


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
	while (s && *s)
		write(2, s++, 1);
	write(2, "\n", 1);
}

int     main(int ac, char **av, char **env)
{
	int		pid_1, pid_2, pid_3, pid_4;
	int		i, j;
	int		fd_1[2], fd_2[2], fd_3[2], infile, outfile;
	int		wait_status, status_code;
	char	*cmd_1, *cmd_2, *cmd_3, *cmd_4, **argv;
	int		flag = 0;

	pipe(fd_1);
	pipe(fd_2);
	infile = open(av[1], O_RDONLY);
	if (infile == -1)
		infile = open("/dev/null", O_RDONLY);
	pid_1 = fork();
	if (pid_1 == 0)
	{
		dup2(infile, 0);
		dup2(fd_1[1], 1);
		close(fd_1[0]);
		close(fd_1[1]);
		close(fd_2[0]);
		close(fd_2[1]);
		argv = ft_split(av[2], ' ');
		cmd_1 = check_cmd_env(argv[0], env);
		cmd_1 = ft_strjoin(cmd_1, argv[0]);
		execve(cmd_1, argv, env);
	}
	else
		close(infile);

	pid_2 = fork();
	if (pid_2 == 0)
	{
		dup2(fd_1[0], 0);
		dup2(fd_2[1], 1);
		close(fd_1[0]);
		close(fd_1[1]);
		close(fd_2[0]);
		close(fd_2[1]);
		argv = ft_split(av[3], ' ');
		cmd_2 = check_cmd_env(argv[0], env);
		cmd_2 = ft_strjoin(cmd_2, argv[0]);
		execve(cmd_2, argv, env);
	}

	outfile = open(av[5], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (outfile == -1)
	{
		perror(NULL);
		flag = 1;
	}
	pid_3 = fork();
	if (pid_3 == 0)
	{
		if (flag == 1)
			exit(1);
		dup2(fd_2[0], 0);
		dup2(outfile, 1);
		close(fd_1[0]);
		close(fd_1[1]);
		close(fd_2[0]);
		close(fd_2[1]);
		argv = ft_split(av[4], ' ');
		cmd_3 = check_cmd_env(argv[0], env);
		cmd_3 = ft_strjoin(cmd_3, argv[0]);
		execve(cmd_3, argv, env);
	}
	else
		close(outfile);

	close(fd_1[0]);
	close(fd_1[1]);
	close(fd_2[0]);
	close(fd_2[1]);
	waitpid(pid_3, &wait_status, 0);
	waitpid(pid_2, NULL, 0);
	waitpid(pid_1, NULL, 0);
	status_code = WEXITSTATUS(wait_status);
	exit(status_code);
	return (0);
}