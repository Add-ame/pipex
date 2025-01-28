#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>


typedef struct s_data
{
    int     fd[2];
    int     fd_1;
    int     fd_2;
    int     wait_status;
    int     status_code;
    char    *cmd_1;
    char    *cmd_2;
    char    **argv;
    int     flag;
    int     pid_1;
    int     pid_2;
    int     i;
    int     j;
}   t_data;

char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strlen(const char *s);
int	c_w(char const *s, char c);

#endif