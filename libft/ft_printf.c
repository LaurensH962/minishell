/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 11:10:22 by lhaas             #+#    #+#             */
/*   Updated: 2025/01/07 09:37:11 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_putstr(char *s, int count)
{
	size_t	i;
	int		ret;

	if (!s)
	{
		ret = write(1, "(null)", 6);
		if (ret == -1)
			return (-1);
		count += ret;
		return (count);
	}
	i = 0;
	while (s[i])
	{
		ret = write(1, &s[i], 1);
		if (ret == -1)
			return (-1);
		count += ret;
		i++;
	}
	return (count);
}

static int	check_specifier(const char str_c, va_list args, int count)
{
	if (str_c == 'd' || str_c == 'i')
		count = ft_putnbr_printf(va_arg(args, int), count);
	else if (str_c == 'x')
		count = ft_putnbr_hex(va_arg(args, int), count, 0);
	else if (str_c == 'X')
		count = ft_putnbr_hex(va_arg(args, int), count, 1);
	else if (str_c == 'u')
		count = ft_putnbr_hex(va_arg(args, int), count, 2);
	else if (str_c == 'p')
		count = ft_putptr(va_arg(args, char *), count);
	else if (str_c == 's')
		count = ft_putstr(va_arg(args, char *), count);
	else if (str_c == 'c')
		count = ft_putchar_printf(va_arg(args, int), count);
	else if (str_c == '%')
	{
		if (write(1, "%", 1) == -1)
			return (-1);
		count++;
	}
	return (count);
}

static int	is_specif(const char str_c)
{
	if (str_c == 'd' || str_c == 'i' || str_c == 'x' || str_c == 'X'
		|| str_c == 'u' || str_c == 'p' || str_c == 's' || str_c == 'c'
		|| str_c == '%')
		return (1);
	return (0);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;
	int		i;

	va_start(args, str);
	i = 0;
	count = 0;
	if (!str)
		return (-1);
	while (str[i] != '\0')
	{
		if (str[i] == '%' && is_specif(str[i + 1]))
			count = check_specifier(str[++i], args, count);
		else if (str[i] == '%' && !str[i + 1])
			count = -1;
		else
			count = ft_putchar_printf(str[i], count);
		if (count == -1)
			break ;
		i++;
	}
	va_end(args);
	return (count);
}
