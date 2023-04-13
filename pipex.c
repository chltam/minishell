/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:27:32 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/10 16:54:30 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(char *str)
{
	write(2, "\033[0;31m", 8);
	write(2, str, ft_strlen(str));
	write(2, "\033[0;39m", 8);
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

void	child_labour(int *fd, t_cmd *cmd, t_envp *envl)
{
	char	**envp;
	char	*path;

	if (dup2(fd[1], STDOUT_FILENO) == -1)
		ft_error("dup2 failed(child)");
	handle_redir_child(cmd);
	close(fd[0]);
	if (is_builtin(cmd->cmd))
		child_is_builtin(cmd, envl);
	envp = envl_to_envp(envl);
	if (access(cmd->args[0], F_OK) == 0)
		path = cmd->args[0];
	else
		path = get_path(cmd->args[0], envp);
	execve(path, cmd->args, envp);
	free(path);
	free_split_in(envp);
	ft_error("child survived");
}

void	child_factory(int *fd, t_cmd *cmd, t_envp *envl)
{
	int		child_id;

	child_id = fork();
	if (child_id == -1)
		ft_error("fork failed");
	if (child_id == 0)
		child_labour(fd, cmd, envl);
	else
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
			ft_error("dup2 error");
		close(fd[1]);
	}
	return ;
}

void	parent_labour(t_cmd_arr *cmd_arr, t_envp *envl)
{
	int		exit_check;
	char	**envp;
	char	*path;

	exit_check = 0;
	handle_redir_parent(cmd_arr);
	if (is_builtin(cmd_arr->cmds[cmd_arr->size - 1]->cmd))
	{
		exit_check = exec_builtin(cmd_arr->cmds[cmd_arr->size - 1],
				envl, false);
		if (!exit_check)
			exit(EXIT_SUCCESS);
		else
			exit(EXIT_FAILURE);
	}
	envp = envl_to_envp(envl);
	if (access(cmd_arr->cmds[cmd_arr->size - 1]->cmd, F_OK) == 0)
		path = cmd_arr->cmds[cmd_arr->size - 1]->cmd;
	else
		path = get_path(cmd_arr->cmds[cmd_arr->size - 1]->cmd, envp);
	execve(path, cmd_arr->cmds[cmd_arr->size - 1]->args, envp);
	free(path);
	free_split_in(envp);
	ft_error("parent survived");
}

void	pipex(t_cmd_arr *cmd_arr, t_envp *envl)
{
	int		fd[2];
	int		n;

	n = 0;
	while (n < cmd_arr->size - 1)
	{
		if (pipe(fd) == -1)
			ft_error("pipe failed");
		child_factory(fd, cmd_arr->cmds[n], envl);
		n++;
	}
	parent_labour(cmd_arr, envl);
}
