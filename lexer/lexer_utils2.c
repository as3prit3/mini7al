/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 14:14:16 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/04 18:55:37 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_special(char c)
{
	return (c == '\'' || c == '\"' || c == '|' || c == '<' || c == '$'
			|| c == '>' || ft_space(c) || c == '\n' || c == '\0');
}

int	get_word(t_compo **list, char *line, enum e_state state)
{
	int	i;

	i = 0;
	while (!is_special(line[i]))
		i++;
	add_to_list(list, new_comp(line, i, state, WORD));
	return (i);
}

void	get_quote(t_compo **list, char *line, enum e_state *state)
{
	enum e_state	new_state;
	enum e_type		type;

	if (*line == '\'')
	{
		new_state = IN_SQUOTE;
		type = SQUOTE;
	}
	else if (*line == '\"')
	{
		new_state = IN_DQUOTE;
		type = DQUOTE;
	}
	if (*state == GENERAL)
	{
		add_to_list(list, new_comp(line, 1, *state, type));
		*state = new_state;
	}
	else if (*state == new_state)
	{
		*state = GENERAL;
		add_to_list(list, new_comp(line, 1, *state, type));
	}
	else
		add_to_list(list, new_comp(line, 1, *state, type));
}

int	get_redirection(t_compo **list, char *line, int i, enum e_state state)
{
	int	j;

	j = i;
	if (line[i] == '<')
	{
		if (line[i + 1] == '<')
			add_to_list(list, new_comp(line + (i++), 2, state, HERE_DOC));
		else
			add_to_list(list, new_comp(line + i, 1, state, REDIR_IN));
		i++;
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
			add_to_list(list, new_comp(line + (i++), 2, state, APPEND));
		else
			add_to_list(list, new_comp(line + i, 1, state, REDIR_OUT));
		i++;
	}
	return (i - j);
}

int	get_env(t_compo **list, char *line, enum e_state state)
{
	int			i;
	enum e_type	type1;
	enum e_type	type2;

	i = 1;
	if (state == IN_SQUOTE)
	{
		type1 = WORD;
		type2 = WORD;
	}
	else
	{
		type1 = ENV;
		type2 = EX_STATUS;
	}
	if (line[i] == '?')
		add_to_list(list, new_comp(line, ++i, state, type2));
	else
	{
		while (line[i] && ft_isalnum(line[i]))
			i++;
		add_to_list(list, new_comp(line, i, state, type1));
	}
	return (i);
}
