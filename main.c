/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 14:14:20 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/04 19:16:48 by hhadhadi         ###   ########.fr       */
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

int main()
{
	char	*line;
	t_data	data;

	signal(SIGQUIT, SIG_IGN);
	while (42)
	{
		signal(SIGINT, exc_sig);
		if (ft_readline(&line))
			continue ;
		data.tokens = lexer(line);
		if (!check_syntax(data.tokens))
		{
			printf("procced to parsing then execution\n");
		}
		free_list(data.tokens);
	}
}
