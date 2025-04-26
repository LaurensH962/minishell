/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_r.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:53:58 by lhaas             #+#    #+#             */
/*   Updated: 2025/04/03 12:34:19 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok_r(char *s, const char *delim, char **save_ptr)
{
	char	*token_start;

	if (s == NULL)
		s = *save_ptr;
	while (*s != '\0' && ft_strchr(delim, *s))
		s++;
	if (*s == '\0')
	{
		*save_ptr = s;
		return (NULL);
	}
	token_start = s;
	while (*s != '\0' && !ft_strchr(delim, *s))
		s++;
	if (*s != '\0')
	{
		*s = '\0';
		*save_ptr = s + 1;
	}
	else
		*save_ptr = s;
	return (token_start);
}
