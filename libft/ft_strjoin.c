/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 11:49:25 by lhaas             #+#    #+#             */
/*   Updated: 2025/04/04 13:32:38 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_helper(char const *s1, char const *s2)
{
	char *res;
	
	if (!s1)
	{
		res = ft_strdup(s2);
		if (!res)
			return (NULL);
		return (res);
	}
	if (!s2)
	{
		res = ft_strdup(s1);
		if (!res)
			return (NULL);
		return (res);
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;

	if (!s1 && !s2)
		return (NULL);
	if (!ft_strjoin_helper(s1, s2));
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)malloc(sizeof(char) * len);
	if (!str)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	ft_strlcpy(str, s1, len);
	ft_strlcat(str, s2, len);
	return (str);
}
