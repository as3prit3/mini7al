/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:16:59 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/05/27 14:29:45 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_lexer	*init_list(t_lexer *list)
{
	list = ft_calloc(sizeof(t_lexer), 1);
	if (!list)
		return (NULL);
	return (list);
}

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

void	add_to_list(t_lexer *lst, t_compo *new)
{
	if (!lst->head)
		lst->head = new;
	else
	{
		lst->tail->next = new;
		new->prev = lst->tail;
	}
	lst->tail = new;
	lst->size++;
}

void	free_list(t_lexer *list)
{
	t_compo	*current;
	t_compo	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	free(list);
}
