/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:11:56 by lhaas             #+#    #+#             */
/*   Updated: 2025/03/06 15:18:07 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t len)
{
	char	*p;

	p = (char *)malloc(len + 1);
	if (p == NULL)
		return (NULL);
	ft_memcpy(p, s, len);
	p[len] = '\0';
	return (p);
}
