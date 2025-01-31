#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>

typedef struct s_data
{
	char	**argv;
	char	**pt;
	char	*tmp;
	char	*cmd;
	char	*cmd_1;
	char	*cmd_2;
	int		fd[2];
	int		fd_1;
	int		fd_2;
	int		pid_1;
	int		pid_2;
	int		flag;
	int		finded;
	int		wait_status;
	int		status_code;
	int		j;
	int		i;
	int		pr;

	int		sp_index;
	int		sp_i;
	int		sp_j;
	int		sp_k;
	int		sp_f;
	int		size;
}	t_data;

char	**ft_split(char const *s, char c, t_data *d);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strlen(const char *s);
int		c_w(char const *s, char c);

int		empty(const char *s);
void	free_pt(char **pt);
int		ft_strlen(const char *s);
void	ft_putstr(char *s, int fd, int flag);
void	put_fd_err(t_data *d, char **av, int fd);

int		only_space(char *s);

#endif