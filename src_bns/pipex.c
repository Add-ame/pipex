#include "pipex.h"

void	init(t_data *d, int ac, char **av, char **env)
{
	d->num_cmds = ac - 3;
	if (d->num_cmds <= 1)
		exit(22);
	d->num_pipes = d->num_cmds - 1;
	d->i = 0;
	while (d->i < d->num_pipes)
		pipe(d->fd[d->i++]);
	d->argv = NULL;
	d->pt = NULL;
	d->tmp = NULL;
	d->cmd = NULL;
	d->cmd_1 = NULL;
	d->cmd_2 = NULL;
	d->infile = 0;
	d->outfile = 0;
	d->pid_1 = 0;
	d->pid_2 = 0;
	d->flag = 0;
	d->finded = 0;
	d->wait_status = 0;
	d->status_code = 0;
	d->j = 0;
	d->i = 0;
}

char	*check_cmd_env(t_data *d, char *argv, char **env)
{
	if (!argv || !ft_strlen(argv))
		return (NULL);
	d->i = 0;
	d->finded = 0;
	while (++d->i >= 0 && env[d->i])
		if (strncmp("PATH=", env[d->i], 5) == 0 && ++d->finded == 1)
			break ;
	d->pt = ft_split(env[d->i] + 5, ':', d);
	d->finded = 0;
	d->i = -1;
	while (++d->i >= 0 && d->pt[d->i])
	{
		d->tmp = d->pt[d->i];
		d->pt[d->i] = ft_strjoin(d->tmp, "/");
		free(d->tmp);
		d->cmd = ft_strjoin(d->pt[d->i], argv);
		if (access(d->cmd, F_OK | X_OK) == 0 && ++d->finded)
			break ;
		free(d->cmd);
	}
	if (!d->finded)
		return (free_pt(d->pt), NULL);
	return (d->pt[d->i]);
}

void	close_fd(t_data *d, int num_pipes, int flag)
{
	d->j = 0;
	while (d->j < num_pipes - flag)
	{
		close(d->fd[d->j][0]);
		close(d->fd[d->j][1]);
		d->j++;
	}
}

void	cmd1(t_data *d, char **av, char **env)
{
	d->infile = open(av[1], O_RDONLY);
	if (d->infile == -1)
		(put_fd_err(d, av, 1)), d->infile = open("/dev/null", O_RDONLY);
	d->pid_1 = fork();
	if (d->pid_1 == 0)
	{
		if (!ft_strlen(av[2]))
			(ft_putstr("permission denied:", 2, 1), exit(126));
		(dup2(d->infile, 0), dup2(d->fd[0][1], 1));
		(close_fd(d, d->num_pipes, 0), d->argv = ft_split(av[2], ' ', d));
		d->cmd_1 = check_cmd_env(d, d->argv[0], env);
		if (!d->cmd_1)
		{
			if (!only_space(av[2]))
				(ft_putstr("command not found: ", 2, 1), free_pt(d->argv), \
				exit(127));
			ft_putstr("command not found: ", 2, 0);
			(ft_putstr(d->argv[0], 2, 1), free_pt(d->argv), exit(127));
		}
		d->cmd_1 = ft_strjoin(d->cmd_1, d->argv[0]);
		execve(d->cmd_1, d->argv, env);
	}
	else
		close(d->infile);
}

void	cmd2(t_data *d, char **av, char **env)
{
	d->outfile = open(av[d->i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (d->outfile == -1)
	{
		put_fd_err(d, av, 2);
		d->flag = 1;
	}
	d->pid_1 = fork();
	if (d->pid_1 == 0)
	{
		if (!ft_strlen(av[d->i]))
			(ft_putstr("permission denied:", 2, 1), exit(126));
		if (d->flag == 1)
			exit(1);
		dup2(d->fd[d->i - 3][0], 0);
		dup2(d->outfile, 1);
		close_fd(d, d->num_pipes, 0);
		d->argv = ft_split(av[d->i], ' ', d);
		d->cmd_2 = check_cmd_env(d, d->argv[0], env);
		if (!d->cmd_2)
			(ft_putstr("command not found: ", 2, 0), ft_putstr(d->argv[0], 2, \
			1), free_pt(d->argv), exit(127));
		d->cmd_2 = ft_strjoin(d->cmd_2, d->argv[0]);
		execve(d->cmd_2, d->argv, env);
	}
	else
		close(d->outfile);
}

void	pipe_them(t_data *d, char **av, char **env)
{
	d->i = 3;
	while (d->i <= d->num_cmds)
	{
		d->pid_1 = fork();
		if (d->pid_1 == 0)
		{
			if (!ft_strlen(av[d->i]))
				(ft_putstr("permission denied:", 2, 1), exit(126));
			(dup2(d->fd[d->i - 3][0], 0));
			dup2(d->fd[d->i - 2][1], 1);
			close_fd(d, d->num_pipes, 0);
			d->argv = ft_split(av[d->i], ' ', d);
			d->cmd_1 = check_cmd_env(d, d->argv[0], env);
			d->cmd_1 = ft_strjoin(d->cmd_1, d->argv[0]);
			if (!d->cmd_1)
				(ft_putstr("command not found: ", 2, 0), ft_putstr(d->argv[0], 2, \
				1), free_pt(d->argv), exit(127));
			execve(d->cmd_1, d->argv, env);
		}
		d->i++;
	}
	close_fd(d, d->num_pipes, 1);
	while (wait(NULL) > 0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*(unsigned char *)s1 - *(unsigned char *)s2);
		s1++;
		s2++;
	}
	return (0);
}

void	here_doc(t_data *d, char **av, char **env)
{
	char	*s;

	d->here_doc = open("d->here_doc", O_CREAT | O_WRONLY | O_APPEND, 0777);
	while (1)
	{
		write(1, "pipe heredoc> ", 14);
		s = get_next_line(0);
		if (!s || ft_strcmp(s, av[2]) == 0)
			break;
		write(d->here_doc, s, ft_strlen(s));
		free(s);
	}

	pipe(d->fd[0]);
	d->pid_1 = fork();
	if (d->pid_1 == 0)
	{
		d->here_doc = open("d->here_doc", O_RDONLY | O_APPEND);
		if (!ft_strlen(av[3]))
			(ft_putstr("permission denied:", 2, 1), exit(126));
		dup2(d->here_doc, 0), dup2(d->fd[0][1], 1);
		(close(d->fd[0][0]), close(d->fd[0][1]));
		d->argv = ft_split(av[3], ' ', d);
		d->cmd_1 = check_cmd_env(d, d->argv[0], env);
		if (!d->cmd_1)
		{
			if (!only_space(av[3]))
				(ft_putstr("command not found: ", 2, 1), free_pt(d->argv), \
				exit(127));
			ft_putstr("command not found: ", 2, 0);
			(ft_putstr(d->argv[0], 2, 1), free_pt(d->argv), exit(127));
		}
		d->cmd_1 = ft_strjoin(d->cmd_1, d->argv[0]);
		execve(d->cmd_1, d->argv, env);
	}
	else
		close(d->here_doc);

	d->outfile = open(av[5], O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (d->outfile == -1)
	{
		put_fd_err(d, av, 2);
		d->flag = 1;
	}
	d->pid_2 = fork();
	if (d->pid_2 == 0)
	{
		if (!ft_strlen(av[4]))
			(ft_putstr("permission denied:", 2, 1), exit(126));
		if (d->flag == 1)
			exit(1);
		(dup2(d->fd[0][0], 0), dup2(d->outfile, 1));
		(close(d->fd[0][0]), close(d->fd[0][1]));
		d->argv = ft_split(av[4], ' ', d);
		d->cmd_2 = check_cmd_env(d, d->argv[0], env);
		if (!d->cmd_2)
			(ft_putstr("command not found: ", 2, 0), ft_putstr(d->argv[0], 2, \
			1), free_pt(d->argv), exit(127));
		d->cmd_2 = ft_strjoin(d->cmd_2, d->argv[0]);
		execve(d->cmd_2, d->argv, env);
	}
	else
		close(d->outfile);

	(close(d->fd[0][0]), close(d->fd[0][1]));
	waitpid(d->pid_1, NULL, 0);
	waitpid(d->pid_2, &d->wait_status, 0);
	d->status_code = WEXITSTATUS(d->wait_status);
	exit(d->status_code);
}

int	main(int ac, char **av, char **env)
{
	t_data	d;

	here_doc(&d, av, env);
	return (0);
	init(&d, ac, av, env);
	cmd1(&d, av, env);
	pipe_them(&d, av, env);
	cmd2(&d, av, env);
	close(d.fd[d.j][0]);
	close(d.fd[d.j][1]);
	wait(&d.wait_status);
	d.status_code = WEXITSTATUS(d.wait_status);
	exit(d.status_code);
	return (0);
}
