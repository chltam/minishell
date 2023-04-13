/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 18:22:46 by htam              #+#    #+#             */
/*   Updated: 2023/04/11 15:08:03 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* global variable */
int	g_exit = 0;

static int	no_pipe_builtin(t_cmd_arr *cmd_arr, t_envp *envl,
	char *input, t_lex *head)
{
	bool	exit;
	int		stdout_backup;

	stdout_backup = dup(STDOUT_FILENO);
	if (cmd_arr->cmds[0]->redir_out == true || cmd_arr->cmds[0]->append == true)
		redir_out(cmd_arr->cmds[0]);
	exit = false;
	g_exit = exec_builtin(cmd_arr->cmds[0], envl, &exit);
	free_cmd_arr(cmd_arr);
	free(input);
	ft_lstclear_lex(&head);
	dup2(stdout_backup, STDOUT_FILENO);
	if (exit == true)
		return (0);
	return (1);
}

static void	init_pipex(t_cmd_arr *cmd_arr, t_envp *envl)
{
	int	pipex_id;
	int	status;

	status = 0;
	signal_action_pipex();
	pipex_id = fork();
	if (pipex_id == -1)
		ft_error("fork failed");
	if (pipex_id == 0)
		pipex(cmd_arr, envl);
	else
		waitpid(pipex_id, &status, 0);
	if (WIFEXITED(status))
		g_exit = WEXITSTATUS(status);
}

static int	input_loop(char *input, t_envp *envl)
{
	t_lex		*head;
	t_cmd_arr	*cmd_arr;

	signal_action();
	input = readline(PROMPT);
	add_history(input);
	if (!input)
		return (0);
	head = ft_lexer(input, envl);
	cmd_arr = parse_lex(head);
	if (!cmd_arr)
	{
		free(input);
		ft_lstclear_lex(&head);
		return (1);
	}
	if (cmd_arr->size == 1 && is_builtin(cmd_arr->cmds[0]->cmd))
		return (no_pipe_builtin(cmd_arr, envl, input, head));
	init_pipex(cmd_arr, envl);
	unlink("heredoc.txt");
	free_cmd_arr(cmd_arr);
	free(input);
	ft_lstclear_lex(&head);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_envp	*envl;
	int		loop;

	(void)argc;
	(void)argv;
	input = NULL;
	loop = 1;
	envl = get_env_list(envp);
	ft_printf(CLEAR_SCR);
	while (1)
	{
		if (input_loop(input, envl) == 0)
			break ;
	}
	ft_lstclear_envl(&envl);
	rl_clear_history();
	return (g_exit);
}
