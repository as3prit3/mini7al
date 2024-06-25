/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 13:18:38 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/25 19:53:59 by hhadhadi         ###   ########.fr       */
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
		if ((*token)->type != ENV && (*token)->type != EX_STATUS)
			free(cmd);
		if (str && (*token)->next && is_redir((*token)->next->type))
			break ;
		*token = (*token)->next;
	}
	return (str);
}

void	in_file(t_data *data, char *str)
{
	data->in = open(str, O_RDONLY, 0644);
	//if (data->in == -1)
		// redir_error(data, str);
}

void	get_redir(t_data *data, t_compo **token, enum e_type type, char *str)
{
	if (str)
		ft_lstadd_back(&data->lst_cmd, ft_lstnew(str));
	str = ft_strdup("");
	*token = (*token)->next;
	if (type == HERE_DOC)
	{}	// here_doc(data, token);
	else
	{
		if ((*token)->type == ENV)
		{
			if (data->expend)
				free(data->expend);
			data->expend = (*token)->content;
		}
		str = get_cmd(data, token, str, type);
		if (type == REDIR_IN)
			in_file(data, str);
		// else if (type == REDIR_OUT)
		// 	out_file(data, str);
		// else if (type == APPEND)
		// 	append(data, str);
	}
	free(str);
}

void	handel_cmd(t_data *data, t_compo **token)
{
	char	*str;

	str = NULL;
	while (*token && !((*token)->type == PIPE && (*token)->state == GENERAL))
	{
		if (is_redir((*token)->type))
			get_redir(data, token, (*token)->type, str);
		else
		{
			if (!str && !ft_space((*token)->type))
				str = ft_strdup("");
			if (!ft_space((*token)->type) && (*token)->state == GENERAL)
				str = get_cmd(data, token, str, WORD);
			if ((str && (!*token || ft_space((*token)->type)))
				|| (*token)->type == PIPE)
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

t_cmd	*ft_cmdnew(int in, int out, t_list *lst_cmd)
{
	t_cmd	*new;
	t_list	*tmp;
	char	**args;
	int		i;

	i = 0;
	tmp = lst_cmd;
	args = malloc(sizeof(char *) * (ft_lstsize(lst_cmd) + 1));
	while (tmp)
	{
		args[i++] = tmp->content;
		tmp = tmp->next;
	}
	args[i] = NULL;
	new = malloc(sizeof(t_cmd));
	new->args = args;
	new->in_file = in;
	new->out_file = out;
	new->next = NULL;
	return (new);
}

void	ft_cmdadd_back(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*cmd)
	{
		*cmd = new;
		return ;
	}
	tmp = *cmd;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	create_cmd(t_data *data)
{
	ft_cmdadd_back(&data->cmd, ft_cmdnew(data->in, data->out, data->lst_cmd));
	data->in = 0;
	data->out = 1;
}

void	free_2d_array(t_cmd *lst, void (*del)(char **))
{
	if (lst && del)
	{
		del(lst->args);
		free(lst);
	}
}

void	ft_cmd_clear(t_cmd **lst)
{
	t_cmd	*tmp;

	if (lst)
	{
		while (*lst)
		{
			tmp = (*lst)->next;
			int	i = 0;
			while ((*lst)->args[i])
				free((*lst)->args[i++]);
			*lst = tmp;
		}
	}
}

void	parse(t_data *data, t_compo *token)
{
	token = skip_spaces(token, 1);
	while (token)
	{
		handel_cmd(data, &token);
		if (!token || token->type == PIPE)
		{
			// create the command list for the execution
			create_cmd(data);
			t_cmd	*cmd = data->cmd;
			int i = 0;
			while (cmd)
			{
				while (cmd->args[i])
					printf("%s\n", cmd->args[i++]);
					// printf("in: %d\n", cmd->in_file);
					// printf("out: %d\n", cmd->out_file);
				cmd = cmd->next;
			}
		}
		if (token)
			token = token->next;
	}
}
