/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhaas <lhaas@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:17:04 by lhaas             #+#    #+#             */
/*   Updated: 2024/11/11 10:12:22 by lhaas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lst_new;
	t_list	*obj_new;
	void	*apply_f;

	if (!lst || !f || !del)
		return (NULL);
	lst_new = NULL;
	while (lst)
	{
		apply_f = f(lst->content);
		obj_new = ft_lstnew(apply_f);
		if (!obj_new)
		{
			ft_lstclear(&lst_new, del);
			del(apply_f);
			return (NULL);
		}
		ft_lstadd_back(&lst_new, obj_new);
		lst = lst->next;
	}
	return (lst_new);
}
