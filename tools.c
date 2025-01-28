#include "pipex.h"

int	c_w(char const *s, char c)
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			size++;
		while (s[i] != c && s[i])
			i++;
	}
	return (size);
}

static char	**ft_free(char **pt, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free(pt[i++]);
	free(pt);
	return (NULL);
}

static char	**ft_strdup_split(char **pt, char const *s, char c, int size)
{
	int	index;
	int	i;
	int	j;
	int	k;
	int	f;

	i = 0;
	index = 0;
	f = 0;
	while (index < size)
	{
		while (s[i] == c)
			i++;
		j = 0;
		if (s[i] == '\'')
		{
			f++;
			i++;
			while (s[i + j] != '\'' && s[i + j])
				j++;	
		}
		else
			while (s[i + j] != c && s[i + j])
				j++;
		pt[index] = (char *)malloc(j + 1);
		if (!pt[index])
			return (ft_free(pt, index));
		k = 0;
		while (k < j)
			pt[index][k++] = s[i++];
		if (f == 1)
			i++;
		pt[index][k] = '\0';
		index++;
	}
	pt[index] = NULL;
	return (pt);
}

char	**ft_split(char const *s, char c)
{
	char	**pt;
	int		size;

	if (!s)
		return (NULL);
	if (c_w(s, '\'') >= 2)
		size = c_w(s, c) - 1;
	else
		size = c_w(s, c);
	// printf("size = %d\n", size);
	pt = (char **)malloc(sizeof(char *) * (size + 1));
	if (!pt)
		return (NULL);
	if (!ft_strdup_split(pt, s, c, size))
		return (NULL);
	return (pt);
}

int		ft_strlen(const char *s)
{
	int		len;

	if (!s)
		return (0);
	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*pt;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	pt = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (pt == NULL)
		return (NULL);
	i = 0;
	while (*s1)
		pt[i++] = *s1++;
	while (*s2)
		pt[i++] = *s2++;
	pt[i] = '\0';
	return (pt);
}