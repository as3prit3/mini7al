/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:18:38 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/06 22:16:14 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*parse_word(t_compo **token)
{
	char	*str;

	str = ft_strdup((*token)->content);
	return (str);
}

char	*parse_env(t_data *data, t_compo **token)
{
	char	*tmp;
	t_env	*env;

	tmp = (*token)->content;
	env = data->envp;
	if ((*token)->type == EX_STATUS)
	{}	// return the exit status and put it in *token->content
	else
	{
		while (env)
		{
			if (ft_strcmp(env->name, tmp + 1) == 0)
			{
				(*token)->content = ft_strdup(env->value);
				break ;
			}
			env = env->next;
		}
		if (!env)
			(*token)->content = ft_strdup("");
	}
	return (free(tmp), (*token)->content);
}

char	*parse_quote(t_data *data, t_compo **tok, enum e_type type)
{
	char	*str;
	char	*tmp;

	str = NULL;
	*tok = (*tok)->next;
	while (*tok)
	{
		if (!str)
			str = ft_strdup("");
		// if ((*tok)->next && (*tok)->type == type && (*tok)->next->type == type)
		// 	*tok = (*tok)->next;
		if ((*tok)->type != type)
		{
			if ((*tok)->type == ENV || (*tok)->type == EX_STATUS)
				parse_env(data, tok);
			tmp = str;
			str = ft_strjoin(tmp, (*tok)->content);
			free(tmp);
		}
		else
			break ;
		if (*tok)
			*tok = (*tok)->next;
	}
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
		if ((*token)->type == SQUOTE || (*token)->type == DQUOTE)
			cmd = parse_quote(data, token, (*token)->type);
		if ((*token)->type == ENV || (*token)->type == EX_STATUS)
			cmd = parse_env(data, token);
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
		if (str && (*token)->next && is_redir((*token)->next->type))
			break ;
		if ((*token)->type != ENV && (*token)->type != EX_STATUS)
			free(cmd);
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
				str = ft_strdup("");
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
		// if (!token || token->type == PIPE)
		// {
		// 	// create the command list for the execution
		// }
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
