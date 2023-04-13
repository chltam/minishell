#include "minishell.h"

void	ft_error(char *str)
{
	write(2, "\033[0;31m", 8);
	write(2, str, ft_strlen(str));
	write(2, "\033[0;39m", 8);
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

void	childfactory(int *fd, t_cmd *cmd, char **envp, t_envp *envl)
{
	int		child_id;
	int		exit_check;

	exit_check = 0;
	// handle redir in and out for child
	child_id = fork();
	if (child_id == -1)
		ft_error("fork failed");
	if (child_id == 0)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			ft_error("dup2 failed(child)");
		close(fd[0]);
		printf("CMD CHILD: %s\n", cmd->cmd);
		if (is_builtin(cmd->cmd))
		{
			printf("IS BUILTIN CHILD\n");
			exit_check = exec_builtin(cmd->args, envl);
			if (!exit_check)
				exit(EXIT_SUCCESS);
			else
				exit(EXIT_FAILURE);
		}
		char **envpp = envl_to_envp(envl);
		execve(get_path(cmd->args[0], envpp), cmd->args, envpp);
		ft_error("child survived");
	}
	else
	{
		waitpid(child_id, NULL, 0);
		if (dup2(fd[0], STDIN_FILENO) == -1)
			ft_error("dup2 error");
		close(fd[1]);
	}
	return ;
}

void	parentlabour(t_cmd_arr *cmd_arr, char **envp, t_envp *envl)
{
	int		outfile_fd;
	int		exit_check;

	exit_check = 0;
	if (cmd_arr->cmds[cmd_arr->size - 1]->redir_out == true)
	{
		outfile_fd = open(cmd_arr->cmds[cmd_arr->size - 1]->out_file,
			O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (outfile_fd == -1)
			ft_error("open outfile failed");
		if (dup2(outfile_fd, STDOUT_FILENO) == -1)
			ft_error("dup2 failed");
	}
	printf("CMD PARENT: %s\n", cmd_arr->cmds[cmd_arr->size - 1]->cmd);
	if (is_builtin(cmd_arr->cmds[cmd_arr->size - 1]->cmd))
	{
		printf("IS BUILTIN PARENT\n");
		exit_check = exec_builtin(cmd_arr->cmds[cmd_arr->size - 1]->args, envl);
		if (!exit_check)
			exit(EXIT_SUCCESS);
		else
			exit(EXIT_FAILURE);
	}
	char	**envpp =  envl_to_envp(envl);
	execve(get_path(cmd_arr->cmds[cmd_arr->size - 1]->cmd, envpp), // leaks from getpath if cmd not found!!
		cmd_arr->cmds[cmd_arr->size - 1]->args, envpp);
	ft_error("parent survived");
}

void	pipex(t_cmd_arr *cmd_arr, char *envp[], t_envp *envl)
{
	int		fd[2];
	int		n;
	int		infile_fd;

	n = 0;
	// handle differently: for every command in and out!
	if (cmd_arr->cmds[0]->redir_in == true)
	{
		infile_fd = open(cmd_arr->cmds[0]->in_file, O_RDONLY);
		if (infile_fd == -1)
			ft_error("open infile failed");
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			ft_error("dup2 error");
	}
	while (n < cmd_arr->size - 1)
	{
		if (pipe(fd) == -1)
			ft_error("pipe failed");
		childfactory(fd, cmd_arr->cmds[n], envp, envl);
		n++;
	}
	wait(NULL);
	parentlabour(cmd_arr, envp, envl);
	exit(EXIT_SUCCESS);
}

// int	pipex_hub(t_cmd_arr *cmd_arr, char *envp[])
// {
// 	t_arg	arg;

// 	arg.cmd_arr = cmd_arr;
// 	arg.envp = envp;
// 	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
// 	{
// 		if (argc < 6)
// 			ft_error("argc error");
// 		medic(arg);
// 	}	
// 	else
// 		pipex(arg);
// 	return (0);
// }
