/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hhadhadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 13:56:40 by hhadhadi          #+#    #+#             */
/*   Updated: 2024/05/27 18:51:18 by hhadhadi         ###   ########.fr       */
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

typedef struct s_lexer
{
	t_compo	*head;
	t_compo	*tail;
	int		size;
}			t_lexer;

	/*lexer_utils*/
bool	is_special(char c);
t_lexer	*init_list(t_lexer *list);
t_compo	*new_comp(char *content, int i, enum e_state state, enum e_type type);
void	add_to_list(t_lexer *lst, t_compo *new);
void	free_list(t_lexer *list);

	/*lexer*/
int		get_word(t_lexer *list, char *line, enum e_state state);
void	get_quote(t_lexer *list, char *line, enum e_state *state);
int		get_redirection(t_lexer *list, char *line, int i, enum e_state state);
int		get_env(t_lexer *list, char *line, enum e_state state);
t_lexer	*lexer(char *line);

	/*syntax check*/
bool	check_syntax(t_lexer *tokenes_lst);
t_compo	*skip_spaces(t_compo *node, int direction);
bool	is_redir(enum e_type type);
#endif
