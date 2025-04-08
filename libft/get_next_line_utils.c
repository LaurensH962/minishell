/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 12:05:56 by lhaas             #+#    #+#             */
/*   Updated: 2024/12/11 14:59:04 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	gnl_ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
	{
		len++;
	}
	return (len);
}

void	*gnl_ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*csrc;
	unsigned char	*cdest;
	size_t			i;

	if (!dest && !src)
		return (NULL);
	csrc = (unsigned char *)src;
	cdest = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		cdest[i] = csrc[i];
		i++;
	}
	return (cdest);
}

char	*gnl_ft_strdup(const char *s)
{
	char	*p;
	size_t	len;

	len = gnl_ft_strlen(s) + 1;
	p = (char *)malloc(len);
	if (p == NULL)
		return (NULL);
	if (p)
		gnl_ft_memcpy(p, s, len);
	return (p);
}

char	*gnl_ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			cc;

	i = 0;
	cc = (char)c;
	while (s[i])
	{
		if (s[i] == cc)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == cc)
		return ((char *)&s[i]);
	return (NULL);
}

size_t	gnl_ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	count;
	size_t	d_len;
	size_t	s_len;

	if (size == 0 && (!dst || !src))
		return (0);
	d_len = gnl_ft_strlen(dst);
	s_len = gnl_ft_strlen(src);
	if (size == 0)
		return (s_len);
	count = 0;
	if (size <= d_len)
		return (s_len + size);
	else if (size > d_len)
	{
		while (count < (size - d_len - 1) && src[count] != '\0')
		{
			dst[d_len + count] = src[count];
			count++;
		}
	}
	dst[d_len + count] = '\0';
	return (d_len + s_len);
}
