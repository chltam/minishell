#include "minishell.h"

static void	run_execmd(t_cmd *cmd);
static void	run_redircmd(t_cmd *cmd);
static void	run_pipecmd(t_cmd *cmd);

static void	run_execmd(t_cmd *cmd)
{
	t_execmd	*ecmd;

	ecmd = (t_execmd *)cmd;
	if (!ecmd->argv[0])
		exit(1);
	// ecmd->path = check_cmd...
	if (execve(ecmd->path, ecmd->argv, NULL) == -1)
		perror("Could not execve");
}

static void	run_redircmd(t_cmd *cmd)
{
	t_redircmd	*rcmd;

	rcmd = (t_redircmd *)cmd;
	close(rcmd->fd);
	if (open(rcmd->file, rcmd->mode) < 0)
	{
		perror("Could not open file");
		exit(1);
	}
	run_cmd(rcmd->cmd);
}

static void	run_pipecmd(t_cmd *cmd)
{
	int			fd[2];
	t_pipecmd	*pcmd;

	pcmd = (t_pipecmd *)cmd;
	if (pipe(fd) < 0)
		exit(1);
	if (ft_fork() == 0) // we are in the 1st child
	{
		close(1);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		run_cmd(pcmd->left);
	}
	if (ft_fork() == 0) // we are in the 1st child
	{
		close(0);
		dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
		run_cmd(pcmd->right);
	}
	close(fd[0]);
	close(fd[1]);
	wait(0);
	wait(0);
}

void	run_cmd(t_cmd *cmd)
{
	if (!cmd)
		exit(1);
	if (cmd->type == EXEC)
		run_execmd(cmd);
	if (cmd->type == REDIR)
		run_redircmd(cmd);
	if (cmd->type == PIPE)
		run_pipecmd(cmd);
	exit(0);
}


// from pipex 

void	exec_cmd(char **av, char **paths, int cmd_opt)
{
	char	**cmd;
	char	*path;
	int		i;

	i = -1;
	cmd = ft_split(av[cmd_opt], ' ');
	if (!cmd)
	{
		free_paths(paths);
		exit(1);
	}
	path = check_cmd(cmd[0], paths);
	if (!path)
	{
		while (cmd[++i] != NULL)
			free(cmd[i]);
		free(cmd);
		free_paths(paths);
		exit(1);
	}
	if (execve(path, cmd, NULL) == -1)
	{
		perror("Could not execve");
		exit(1);
	}
}

char	*check_cmd(char *cmd, char **paths)
{
	int		i;
	char	*cmd_path;
	char	*cmd_path2;

	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(paths[i], "/");
		if (!cmd_path)
			return (NULL);
		cmd_path2 = ft_strjoin(cmd_path, cmd);
		free(cmd_path);
		if (!cmd_path2)
			return (NULL);
		if (access(cmd_path2, X_OK) == 0)
			return (cmd_path2);
		i++;
		free(cmd_path2);
	}
	return (NULL);
}

static char	*get_path_str(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH", envp[i], 4) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	**get_paths(char **envp)
{
	char	**paths;
	char	*path_str;

	path_str = get_path_str(envp);
	paths = ft_split(path_str, ':');
	return (paths);
}
