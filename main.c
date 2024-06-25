/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:14:20 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/25 19:53:03 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

bool	ft_readline(char **line)
{
	*line = readline("[minishell]~>");
	if(!*line || ft_strcmp(*line, "exit") == 0)
	{
		printf("exit\n");
		free(*line);
		exit(1);
	}
	if (ft_isspace(*line))
	{
		free(*line);
		return (1);
	}
	if (ft_strlen(*line) > 0)
		add_history(*line);
	return (0);
}

void	exc_sig(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

int main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	t_exec	exec;

	(void)ac;
	(void)av;
	data = (t_data){0};
	create_env(&exec, env);
	data.envp = exec.env;
	signal(SIGQUIT, SIG_IGN);
	while (42)
	{
		signal(SIGINT, exc_sig);
		if (ft_readline(&line))
			continue ;
		data.tokens = lexer(line);
		if (!check_syntax(data.tokens))
		{
			parse(&data, data.tokens);
			printf("procced to parsing then execution\n");
		}
		// ft_cmd_clear(&data.cmd);
		ft_lstclear(&data.lst_cmd, free);
		free_list(data.tokens);

	}
}
