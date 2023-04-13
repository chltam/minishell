/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htam <htam@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 17:04:29 by htam              #+#    #+#             */
/*   Updated: 2023/04/10 20:42:46 by htam             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
	if (sig == SIGQUIT)
	{
		printf("\e[2P");
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
}

void	signal_action(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &handle_sig;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

static void	handle_sig_pipex(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		g_exit = 130;
		return ;
	}
	if (sig == SIGQUIT)
	{
		printf("Quit\n");
		g_exit = 131;
		return ;
	}
}

void	signal_action_pipex(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = &handle_sig_pipex;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
