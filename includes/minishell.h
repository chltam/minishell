/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 18:24:53 by htam              #+#    #+#             */
/*   Updated: 2023/04/10 20:47:19 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "../libft/includes/libft.h"

# define MAX_ARGS 10
# define CLEAR_SCR " \e[1;1H\e[2J"
# define SET_CURSOR "\e[13C\b\b"
# define PROMPT "MINISHELL: "
# define SYMBOLS "<>|$\"\'"

extern int	g_exit;

/* struct to wrap cmd from lex list */
typedef struct s_cmd {
	char	*cmd;
	bool	arg;
	char	**args;
	bool	redir_in;
	char	*in_file;
	bool	redir_out;
	char	*out_file;
	bool	heredoc;
	char	*delimiter;
	char	*heredoc_txt;
	bool	append;
}	t_cmd;

typedef struct s_cmd_arr {
	t_cmd	**cmds;
	bool	cmds_check;
	int		size;
}			t_cmd_arr;

/* pipex */
typedef struct s_arg {
	t_cmd_arr	*cmd_arr;
	char		**envp;
}	t_arg;

typedef enum e_token
{
	CMD_STR,
	DQUOTE,	
	SQUOTE,	
	REDIR_IN,
	REDIR_OUT,
	DELIMITER,
	APPEND,	
	ENV_VAR,
	PIPE_TOK,
	WHITE_SPACE
}	t_token;

typedef struct s_lex
{
	t_token			token;
	char			*content;
	struct s_lex	*next;
}	t_lex;

typedef struct s_input
{
	char	*cmd;
	char	**arg;
}	t_input;

typedef struct s_envp
{
	char			*name;
	char			*value;
	struct s_envp	*next;
}	t_envp;

/* parsing */
t_cmd_arr	*parse_lex(t_lex *head);
void		parse_more(t_cmd *cmd, t_lex *tmp);
int			get_cmds_size(t_lex *head);
char		*read_heredoc(char *delimiter);
t_cmd		*cmd_constructor(void);
void		check_infile(char *infile);
void		quit_heredoc(char *here, char *delimiter);
void		fill_heredoc(char *here, char *buffer);
void		init_cmd_redir(t_cmd *cmd);

/* pipex */
void		pipex(t_cmd_arr *cmd_arr, t_envp *envl);
char		*get_path(char *cmd, char **envp);
void		ft_error(char *str);
void		child_is_builtin(t_cmd *cmd, t_envp *envl);
void		redir_out(t_cmd *cmd);

/* builtins */
int			exec_builtin(t_cmd *cmd, t_envp *envl, bool *exit);
int			is_builtin(char	*cmd);
void		echo(t_cmd *cmd);
int			change_dir(char **args, t_envp *envl);
void		pwd(void);

/* parsing utility */
int			is_symbol(char c);
char		peek(char *input, int peek_pos);
int			peek_quotes(char *input, char quote, int *peek_pos);

/* other utility */
char		**split_input(char *input);
void		free_split_in(char **split_in);
void		free_cmd(t_cmd *cmd);
void		free_cmd_arr(t_cmd_arr *cmd_arr);
void		handle_redir_child(t_cmd *cmd);
void		handle_redir_parent(t_cmd_arr *cmd_arr);
// void		print_cmd(t_cmd *cmd);

/* list utility */
t_lex		*ft_lstnew_lex(char *str, t_token token);
void		ft_lstclear_lex(t_lex **lst);
void		ft_lstdelone_lex(t_lex *lst);
void		ft_lstadd_back_lex(t_lex **lst, t_lex *new);
t_lex		*ft_lstlast_lex(t_lex *lst);
void		del(void *content);

/*environemt variable related*/
t_envp		*get_env_list(char **envp);
int			ft_env(t_envp *envl);
char		*call_env_var(t_envp *envl, char *name);
int			ft_export(t_envp *envp, char **args);
int			ft_unset(t_envp *envl, char **args);
void		free_env_list(t_envp *envlist);
char		*ft_getenv(t_envp *envl, char *name);
void		ft_setenv(t_envp *envl, char *name, char *value);
char		**envl_to_envp(t_envp *envl);
char		**split_two(char *full);

/*envl utility*/
t_envp		*ft_lstnew_envl(char *name, char *value);
void		ft_lstadd_back_envl(t_envp **lst, t_envp *new);
t_envp		*ft_lstlast_envl(t_envp *lst);
void		ft_lstclear_envl(t_envp **lst);
void		ft_lstdelone_envl(t_envp *lst);

/*lexer*/
t_lex		*ft_lexer(char *input, t_envp *envp);
char		*lexer_space(char *input, t_lex **lex);
char		*lexer_pipe(char *input, t_lex **lex);
char		*lexer_cmd(char *input, t_lex **lex);
char		*lexer_quote(char *input, t_lex **lex);
char		*lexer_env(char *input, t_lex **lex);
char		*lexer_out(char *input, t_lex **lex);
char		*lexer_in(char *input, t_lex **lex);
void		node_merger(t_lex *lex);
void		check_env_var(t_lex *lex, t_envp *envp);
void		check_dquote(t_lex *lex, t_envp *envp);
void		check_redir(t_lex *lex);
void		check_space(t_lex *lex);
t_lex		*mini_lexer(char *input, t_envp *envp);
char		**ft_strtok_pro_max(t_lex *lex);

/*signal*/
void		signal_action(void);
void		signal_action_pipex(void);

/*test*/
void		print_lex(t_lex *lex);

#endif
