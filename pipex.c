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
	t_data	d;

	if (ac != 5)
		exit(22);
	pipe(d.fd);
	d.fd_1 = open(av[1], O_RDONLY);
	if (d.fd_1 == -1)
		d.fd_1 = open("/dev/null", O_RDONLY);
	d.pid_1 = fork();
	if (d.pid_1 == 0)
	{
		dup2(d.fd_1, 0);
		dup2(d.fd[1], 1);
		close(d.fd[0]);
		close(d.fd[1]);
		d.argv = ft_split(av[2], ' ');
		d.cmd_1 = check_cmd_env(d.argv[0], env);
		if (!d.cmd_1)
		{
			put_error(d.argv[0]);
			free_pt(d.argv);
			exit(127);
		}
		d.cmd_1 = ft_strjoin(d.cmd_1, d.argv[0]);
		execve(d.cmd_1, d.argv, env);
	}
	else
		close(d.fd_1);

	d.fd_2 = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (d.fd_2 == -1)
	{
		perror(NULL);
		d.flag = 1;
	}
	d.pid_2 = fork();
	if (d.pid_2 == 0)
	{
		if (d.flag == 1)
			exit(1);
		dup2(d.fd[0], 0);
		dup2(d.fd_2, 1);
		close(d.fd[0]);
		close(d.fd[1]);
		d.argv = ft_split(av[3], ' ');
		d.cmd_2 = check_cmd_env(d.argv[0], env);
		if (!d.cmd_2)
		{
			put_error(d.argv[0]);
			free_pt(d.argv);
			exit(127);
		}
		d.cmd_2 = ft_strjoin(d.cmd_2, d.argv[0]);
		execve(d.cmd_2, d.argv, env);
	}
	else
		close(d.fd_1);

	close(d.fd[0]);
	close(d.fd[1]);
	waitpid(d.pid_2, &d.wait_status, 0);
	waitpid(d.pid_1, NULL, 0);
	d.status_code = WEXITSTATUS(d.wait_status);
	exit(d.status_code);
	return (0);
}
