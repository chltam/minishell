/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:19:20 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/10 16:46:12 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	redir_heredoc(t_cmd *cmd)
{
	int		fd_heredoc;

	fd_heredoc = open("heredoc.txt", O_CREAT | O_RDWR, 0644);
	if (fd_heredoc == -1)
	{
		perror("File could not be created");
		exit(1);
	}
	write(fd_heredoc, cmd->heredoc_txt, ft_strlen(cmd->heredoc_txt));
	close(fd_heredoc);
	fd_heredoc = open("heredoc.txt", O_RDONLY);
	if (fd_heredoc == -1)
		ft_error("open heredoc failed");
	if (dup2(fd_heredoc, STDIN_FILENO) == -1)
		ft_error("dup2 error");
	close(fd_heredoc);
}

void	redir_out(t_cmd *cmd)
{
	int		outfile_fd;

	if (cmd->append)
		outfile_fd = open(cmd->out_file,
				O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		outfile_fd = open(cmd->out_file,
				O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (outfile_fd == -1)
		ft_error("open outfile failed");
	if (dup2(outfile_fd, STDOUT_FILENO) == -1)
		ft_error("dup2 failed");
}

void	handle_redir_child(t_cmd *cmd)
{
	int		infile_fd;

	if (cmd->heredoc == true)
		redir_heredoc(cmd);
	if (cmd->redir_in == true)
	{
		infile_fd = open(cmd->in_file, O_RDONLY);
		if (infile_fd == -1)
			ft_error("open infile failed");
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			ft_error("dup2 error");
	}
	if (cmd->redir_out == true || cmd->append == true)
		redir_out(cmd);
}

void	handle_redir_parent(t_cmd_arr *cmd_arr)
{
	int		infile_fd;
	t_cmd	*cmd;

	cmd = cmd_arr->cmds[cmd_arr->size - 1];
	if (cmd->heredoc == true)
		redir_heredoc(cmd);
	if (cmd->redir_in == true)
	{
		infile_fd = open(cmd->in_file, O_RDONLY);
		if (infile_fd == -1)
			ft_error("open infile failed");
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			ft_error("dup2 error");
	}
	if (cmd->redir_out == true || cmd->append == true)
		redir_out(cmd);
}
