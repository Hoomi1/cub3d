/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyuuki <cyuuki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/06 17:16:16 by cyuuki            #+#    #+#             */
/*   Updated: 2021/04/27 21:06:41 by cyuuki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int c, size_t n)
{
	int		i;
	char	*src;

	src = dest;
	i = 0;
	while (n)
	{
		src[i] = c;
		i++;
		n--;
	}
	return (dest);
}
