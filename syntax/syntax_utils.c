/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:50:04 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/05/27 19:10:57 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_compo	*skip_spaces(t_compo *node, int direction)
{
	while (node && node->type == W_SPACE)
	{
		if (direction)
			node = node->next;
		else
			node = node->prev;
	}
	return (node);
}

bool	is_redir(enum e_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT || type == HERE_DOC
			|| type == APPEND);
}
