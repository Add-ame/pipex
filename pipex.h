#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>



char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);

#endif