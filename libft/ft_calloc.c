/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 09:49:13 by lhaas             #+#    #+#             */
/*   Updated: 2024/11/11 15:23:03 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	all;

	all = nmemb * size;
	if (nmemb != 0 && size != 0 && (size != (all / nmemb)))
		return (NULL);
	p = (void *)malloc(all);
	if (p)
		ft_bzero(p, all);
	return (p);
}
