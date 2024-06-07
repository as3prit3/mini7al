/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:42:08 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/07 17:05:23 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

char	*get_name(char *str, char c)
{
	char	*new;
	int		i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	new = malloc(i + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

t_env	*new_env(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = name;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	add_env(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	env_len(t_env *env)
{
	int		i;

	if (!env)
		return (0);
	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);

}

// char	**list_to_tab(t_env **env)
// {
// 	char	**tab;
// 	t_env	*tmp;
// 	int		i;
// 	char	*str;

// 	i = env_len(*env);
// 	tab = malloc(sizeof(char *) * (i + 1));
// 	if (!tab)
// 		return (NULL);
// 	i = 0;
// 	tmp = *env;
// 	while (tmp)
// 	{
// 		t_env *next = tmp->next;
// 		str = ft_strjoin(tmp->name, "=");
// 		tab[i++] = ft_strjoin(str, tmp->value);
// 		free(str);
// 		tmp = next;
// 	}
// 	tab[i] = NULL;
// 	return (tab);
// }

char	**p_get_env(t_exec *exec, char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	exec->n_env = (char **)malloc(sizeof(char*) * (i + 1));
	if (!exec->n_env)
		return (NULL);
	i = 0;
	while (env && env[i])
	{
		exec->n_env[i] = ft_strdup(env[i]);
		i++;
	}
	exec->n_env[i] = NULL;
	return (exec->n_env);
}

void	create_env(t_exec *exec, char **env)
{
	char	*name;
	char	*value;
	int		i;

	i = -1;
	exec->env = NULL;
	while (env && env[++i])
	{
		name = get_name(env[i], '=');
		value = ft_strchr(env[i], '=') + 1;
		add_env(&exec->env, new_env(name, value));
	}
	exec->n_env = p_get_env(exec, env);
}
