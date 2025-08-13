/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:03:48 by lhaas             #+#    #+#             */
/*   Updated: 2025/05/22 14:29:41 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_skip_space(int *i, const char *nptr)
{
	while (ft_isspace(nptr[*i]))
		(*i)++;
	return (*i);
}

bool	ft_atoi_safe(const char *nptr, long long *out)
{
	unsigned long long	res;
	int					sign;
	int					i;
	int					digit;

	res = 0;
	sign = 1;
	i = 0;
	i = ft_skip_space(&i, nptr);
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign = -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i++] - '0';
		res = res * 10 + digit;
		if ((sign == 1 && res > (unsigned long long)LLONG_MAX)
			|| (sign == -1 && res > (unsigned long long)LLONG_MAX + 1))
			return (false);
	}
	if (sign == 1)
		*out = (long long)res;
	else
		*out = -(long long)res;
	return (true);
}

void	exit_close_pipes(t_shell *shell)
{
	if (shell->pipe_count > 0)
		close_pipes(shell);
}
