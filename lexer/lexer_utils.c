/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:16:59 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/04 19:04:47 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_compo	*new_comp(char *content, int i, enum e_state state, enum e_type type)
{
	t_compo	*new;
	char	*tmp;

	new = ft_calloc(sizeof(t_compo), 1);
	if (!new)
		return (NULL);
	tmp = ft_strndup(content, i);
	if (!tmp)
		return (NULL);
	new->content = tmp;
	new->state = state;
	new->type = type;
	return (new);
}

void	add_to_list(t_compo **lst, t_compo *new)
{
	t_compo	*tmp;

	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}
}

void	free_list(t_compo *list)
{
	t_compo	*current;
	t_compo	*next;

	if (!list)
		return ;
	current = list;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}
