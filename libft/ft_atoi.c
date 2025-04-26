/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:06:51 by lhaas             #+#    #+#             */
/*   Updated: 2025/02/09 13:24:02 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long long	ft_atoi(const char *nptr)
{
	long long	sign;
	long long	res;
	long long	idx;

	sign = 1;
	res = 0;
	idx = 0;
	while (nptr[idx] == ' ' || nptr[idx] == '\n' || nptr[idx] == '\t'
		|| nptr[idx] == '\v' || nptr[idx] == '\f' || nptr[idx] == '\r')
		idx++;
	if (nptr[idx] == '-' || nptr[idx] == '+')
	{
		if (nptr[idx++] == '-')
			sign = -1;
	}
	while (nptr[idx] >= '0' && nptr[idx] <= '9')
	{
		res = 10 * res + (nptr[idx++] - '0');
	}
	return (res * sign);
}
