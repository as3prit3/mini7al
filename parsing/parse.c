/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:18:38 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/05 19:50:53 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*parse_word(t_compo **token)
{
	char	*str;

	str = ft_strdup((*token)->content);
	return (str);
}

char	*parse_cmd(t_data *data, t_compo **token)
{
	char	*cmd;

	(void)data;
	cmd = NULL;
	if (*token)
	{
		if ((*token)->type == WORD)
			cmd = parse_word(token);
		// if ((*token)->type == SQUOTE || (*token)->type == DQUOTE)
		// 	cmd = parse_quote(data, token, (*token)->type);
		// if ((*token)->type == ENV || (*token)->type == EX_STATUS)
		// 	cmd = parse_env(token);
	}
	return (cmd);
}

char	*get_cmd(t_data *data, t_compo **token, char *str, enum e_type type)
{
	char	*tmp;
	char	*cmd;

	(void)type;
	*token = skip_spaces(*token, 1);
	while (*token && !ft_space((*token)->type) && (*token)->type != PIPE)
	{
		cmd = parse_cmd(data, token);
		tmp = str;
		str = ft_strjoin(tmp, cmd);
		free(tmp);
		free(cmd);
		if (str && (*token)->next && is_redir((*token)->next->type))
			break ;
		*token = (*token)->next;
	}
	return (str);
}

void	handel_cmd(t_data *data, t_compo **token)
{
	char	*str;

	str = NULL;
	while (*token && !((*token)->type == PIPE && (*token)->state == GENERAL))
	{
		if (is_redir((*token)->type))
		{}
		else
		{
			if (!str && !ft_space((*token)->type))
				str =ft_strdup("");
			if (!ft_space((*token)->type) && (*token)->state == GENERAL)
				str = get_cmd(data, token, str, WORD);
			if (str && (!*token || ft_space((*token)->type)
				|| (*token)->type == PIPE))
			{
				ft_lstadd_back(&data->lst_cmd, ft_lstnew(str));
				str = NULL;
			}
		}
		if (*token && (*token)->type == PIPE)
			break ;
		if (*token)
			*token = (*token)->next;
	}
}

void	parse(t_data *data, t_compo *token)
{
	token = skip_spaces(token, 1);
	while (token)
	{
		handel_cmd(data, &token);
		if (token && token->type == PIPE)
		{
			// create the command list for the execution
		}
		if (token)
			token = token->next;
	}
	t_list *tmp = data->lst_cmd;
	while (tmp)
	{
		printf("args: %s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
	ft_lstclear(&data->lst_cmd, &free);
}
