/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 13:56:40 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/06/25 19:49:47 by hhadhadi         ###   ########.fr       */
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
	W_SPACE = ' ',
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
	t_list	*lst_cmd;
	char	*expend;
	int		in;
	int		out;
}			t_data;

	/*execution struct*/
typedef struct s_exec
{
	char	**n_env;
	t_env	*env;
}			t_exec;

	/*execution*/
void	create_env(t_exec *exec, char **env);

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

	/*parsing utils*/
void	ft_cmd_clear(t_cmd **lst);
// void	free_2d_array(t_cmd *lst, void (*del)(char **));

	/*parsing*/
void	parse(t_data *data, t_compo *token);
#endif
