/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:45:47 by lhaas             #+#    #+#             */
/*   Updated: 2025/01/07 09:37:03 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar_printf(char c, int count)
{
	int	ret;

	ret = write(1, &c, 1);
	if (ret == -1)
		return (-1);
	return (count + ret);
}

int	ft_putnbr_base(unsigned long long n, int count, char *base,
		unsigned int base_n)
{
	if (count == -1)
		return (-1);
	if (n >= base_n)
	{
		count = ft_putnbr_base(n / base_n, count, base, base_n);
		if (count == -1)
			return (-1);
		count = ft_putnbr_base(n % base_n, count, base, base_n);
	}
	else
	{
		count = ft_putchar_printf(base[n], count);
	}
	return (count);
}

int	ft_putnbr_hex(unsigned int n, int count, int uppercase)
{
	if (count == -1)
		return (-1);
	if (uppercase == 1)
	{
		count = ft_putnbr_base((unsigned long long)n, count, "0123456789ABCDEF",
				16);
	}
	else if (uppercase == 2)
		count = ft_putnbr_base((unsigned long long)n, count, "0123456789", 10);
	else
	{
		count = ft_putnbr_base((unsigned long long)n, count, "0123456789abcdef",
				16);
	}
	return (count);
}

int	ft_putptr(void *ptr, int count)
{
	int	ret;

	if (!ptr)
	{
		ret = write(1, "(nil)", 5);
		if (ret == -1)
			return (-1);
		return (count + ret);
	}
	ret = write(1, "0x", 2);
	if (ret == -1)
		return (-1);
	count += ret;
	count = ft_putnbr_base((unsigned long long)(uintptr_t)ptr, count,
			"0123456789abcdef", 16);
	return (count);
}

int	ft_putnbr_printf(int n, int count)
{
	if (count == -1)
		return (-1);
	if (n == -2147483648)
		return (count + write(1, "-2147483648", 11));
	if (n < 0)
	{
		count += write(1, "-", 1);
		if (count == -1)
			return (-1);
		n = -n;
	}
	if (n > 9)
	{
		count = ft_putnbr_printf(n / 10, count);
		if (count == -1)
			return (-1);
	}
	return (ft_putchar_printf(n % 10 + '0', count));
}
