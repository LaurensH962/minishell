/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:19:27 by lhaas             #+#    #+#             */
/*   Updated: 2025/03/19 15:00:13 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	count;
	size_t	d_len;
	size_t	s_len;

	if (size == 0 && (!dst || !src))
		return (0);
	d_len = ft_strlen(dst);
	s_len = ft_strlen(src);
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
