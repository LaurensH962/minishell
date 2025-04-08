/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:02:20 by lhaas             #+#    #+#             */
/*   Updated: 2024/12/11 14:59:14 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*gnl_ft_strjoin(char *s1, char *s2)
{
	size_t	len;
	char	*result;

	len = gnl_ft_strlen(s1) + gnl_ft_strlen(s2);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	gnl_ft_strlcat(result, s1, len);
	gnl_ft_strlcat(result, s2, len + 1);
	free(s1);
	return (result);
}

struct elements	gnl_find_line(t_elems els, int fd)
{
	static char	*remainder[1024];

	if (remainder[fd])
		els.line = remainder[fd];
	else
		els.line = gnl_ft_strdup("");
	remainder[fd] = NULL;
	while (els.bytes_read > 0)
	{
		els.newline_pos = gnl_ft_strchr(els.line, '\n');
		if (els.newline_pos != NULL)
		{
			remainder[fd] = gnl_ft_strdup(els.newline_pos + 1);
			*(els.newline_pos + 1) = '\0';
			break ;
		}
		els.bytes_read = read(fd, els.buffer, BUFFER_SIZE);
		if (els.bytes_read <= 0)
			break ;
		els.buffer[els.bytes_read] = '\0';
		els.line = gnl_ft_strjoin(els.line, els.buffer);
	}
	return (els);
}

char	*get_next_line(int fd)
{
	t_elems	els;

	els.bytes_read = 1;
	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	els.buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!els.buffer)
		return (NULL);
	els = gnl_find_line(els, fd);
	if (els.bytes_read < 0 || (els.bytes_read == 0 && !*els.line))
		return (free(els.buffer), free(els.line), NULL);
	return (free(els.buffer), els.line);
}
