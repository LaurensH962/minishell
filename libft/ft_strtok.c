/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:01:01 by lhaas             #+#    #+#             */
/*   Updated: 2025/04/01 17:26:56 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *s, const char *delim)
{
	static char	*olds;
	char		*token;

	if (s == NULL)
		s = olds;
	while (*s != '\0' && ft_strchr(delim, *s))
		s++;
	if (*s == '\0')
	{
		olds = s;
		return (NULL);
	}
	token = s;
	while (*s != '\0' && !ft_strchr(delim, *s))
		s++;
	if (*s != '\0')
	{
		*s = '\0';
		olds = s + 1;
	}
	else
		olds = s;
	return (token);
}
