/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:47:30 by lhaas             #+#    #+#             */
/*   Updated: 2024/11/11 15:23:13 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(int n)
{
	int	i;

	i = 0;
	if (n == -2147483648)
		return (11);
	else if (n <= 0)
	{
		i++;
		n = -n;
	}
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static char	*ft_solve(int n, int len, int i, char *res)
{
	if (n == -2147483648)
	{
		ft_strlcpy(res, "-2147483648", 12);
		return (res);
	}
	else if (n < 0)
	{
		res[i++] = '-';
		n = -n;
	}
	res[len] = '\0';
	while (len > i)
	{
		len--;
		res[len] = n % 10 + '0';
		n = n / 10;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	int		i;
	int		len;
	char	*res;

	i = 0;
	len = ft_count(n);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	return (ft_solve(n, len, i, res));
}
