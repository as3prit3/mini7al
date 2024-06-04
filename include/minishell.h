/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 13:56:40 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/04 19:15:54 by hhadhadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"
# include <stdbool.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

enum e_type
{
	WORD,
	W_SPACE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HERE_DOC,
	PIPE,
	ENV,
	SQUOTE,
	DQUOTE,
	EX_STATUS,
};

enum e_state
{
	GENERAL,
	IN_SQUOTE,
	IN_DQUOTE,
};

typedef struct s_compo
{
	char			*content;
	enum e_type		type;
	enum e_state	state;
	struct s_compo	*next;
	struct s_compo	*prev;
}			t_compo;

typedef struct s_cmd
{
	char			**args;
	int				in_file;
	int				out_file;
	struct s_cmd	*next;
}			t_cmd;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}			t_env;

typedef struct s_data
{
	t_cmd	*cmd;
	t_env	*envp;
	t_compo	*tokens;
	t_list	lst_cmd;
}			t_data;

	/*lexer_utils*/
bool	is_special(char c);
t_compo	*new_comp(char *content, int i, enum e_state state, enum e_type type);
void	add_to_list(t_compo **lst, t_compo *new);
void	free_list(t_compo *list);

	/*lexer*/
int		get_word(t_compo **list, char *line, enum e_state state);
void	get_quote(t_compo **list, char *line, enum e_state *state);
int		get_redirection(t_compo **list, char *line, int i, enum e_state state);
int		get_env(t_compo **list, char *line, enum e_state state);
t_compo	*lexer(char *line);

	/*syntax check*/
bool	check_syntax(t_compo *tokenes_lst);
t_compo	*skip_spaces(t_compo *node, int direction);
bool	is_redir(enum e_type type);
#endif
