/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:11:43 by lhaas             #+#    #+#             */
/*   Updated: 2024/11/11 15:23:28 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*p;
	unsigned char	t;
	size_t			i;

	p = (unsigned char *)s;
	t = c;
	i = 0;
	while (i < n)
	{
		if (*p == t)
			return ((unsigned char *)p);
		p++;
		i++;
	}
	return (NULL);
}
