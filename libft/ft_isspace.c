/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 11:03:11 by lhaas             #+#    #+#             */
/*   Updated: 2025/03/06 11:06:32 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isspace(char c)
{
	if (c == '\f' || c == ' ' || c == '\r' || c == '\n' || c == '\t'
		|| c == '\v')
	{
		return (1);
	}
	return (0);
}
