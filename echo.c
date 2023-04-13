/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 18:28:56 by cbadura           #+#    #+#             */
/*   Updated: 2023/04/11 15:50:22 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_echo(char **args, int i, bool newline);
static void	echo_to_file(t_cmd *cmd, int i, bool newline);
static int	check_n(char *arg);

void	echo(t_cmd *cmd)
{	
	bool	newline;
	int		i;

	newline = true;
	if (cmd->args[1])
	{
		if ((ft_strncmp(cmd->args[1], "-n", 2) == 0) && check_n(cmd->args[1]))
		{
			newline = false;
			i = 2;
			if (!cmd->args[2])
				return ;
		}
		else
			i = 1;
		if (cmd->redir_out || cmd->append)
			echo_to_file(cmd, i, newline);
		else
			print_echo(cmd->args, i, newline);
	}
}

static void	print_echo(char **args, int i, bool newline)
{
	int		j;

	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			ft_printf("%c", args[i][j]);
			if (args[i][j])
				j++;
		}
		i++;
		if (args[i])
			ft_printf(" ");
	}
	if (newline == true)
		ft_printf("\n");
}

static void	echo_to_file(t_cmd *cmd, int i, bool newline)
{
	int		outfile_fd;
	int		j;

	if (cmd->append)
		outfile_fd = open(cmd->out_file, O_CREAT | O_WRONLY | O_APPEND, 0777);
	else
		outfile_fd = open(cmd->out_file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (outfile_fd == -1)
		perror("open outfile failed");
	while (cmd->args[i])
	{
		j = 0;
		while (cmd->args[i][j])
		{
			ft_putchar_fd(cmd->args[i][j], outfile_fd);
			if (cmd->args[i][j])
				j++;
		}
		i++;
		if (cmd->args[i])
			ft_putchar_fd(' ', outfile_fd);
	}
	if (newline == true)
		ft_putchar_fd('\n', outfile_fd);
	close(outfile_fd);
}

static int	check_n(char *arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
