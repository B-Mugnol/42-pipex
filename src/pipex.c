/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:40 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/18 19:23:53 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_fd_pair	io_file_opener(char *infilename, char *outfilename);
static t_command	*fetch_commands(int argc, char *argv[], char *envp[],
						t_fd_pair iof);
static void			exec_cmd(int r_fd, int w_fd, t_command *cmd, char *envp[]);
static void			pipex(t_fd_pair iof, t_command *cmd, char *envp[]);

int	main(int argc, char *argv[], char *envp[])
{
	t_command	*cmd;
	t_fd_pair	iof;

	if (param_verifier(argc))
		return (EXIT_FAILURE);
	iof = io_file_opener(argv[1], argv[argc - 1]);
	if (iof.status)
		return (iof.status);
	cmd = fetch_commands(argc, argv, envp, iof);
	//if cmd[i].status && cmd[i + 1].status
	pipex(iof, cmd, envp);
	free_command_vector(argc - 3, &cmd);
	if (iof.fd[0] != -1)
		close(iof.fd[0]);
	if (iof.fd[1] != -1)
		close(iof.fd[1]);
	return (EXIT_FAILURE);
}

static t_fd_pair	io_file_opener(char *infilename, char *outfilename)
{
	t_fd_pair	f;

	f.status = 0;
	f.fd[0] = open(infilename, O_RDONLY);
	if (f.fd[0] == -1 && access(infilename, F_OK) == -1)
		print_coded_error("pipex", infilename, ENOENT);
	else if (f.fd[0] == -1)
		print_coded_error("pipex", infilename, EACCESS);
	f.fd[1] = open(outfilename, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (f.fd[1] == -1)
	{
		if (f.fd[0] != -1)
			close(f.fd[0]);
		print_coded_error("pipex", outfilename, EACCESS);
		f.status = 1;
	}
	return (f);
}

static t_command	*fetch_commands(int argc, char *argv[], char *envp[],
						t_fd_pair iof)
{
	t_command	*cmd;
	int			i;

	cmd = malloc((argc - 3) * sizeof (t_command));
	if (!cmd)
		print_error_exit("pipex: malloc");
	i = -1;
	while (++i < argc - 3)
		cmd[i] = (t_command){.status = -1, .param = NULL, .pathname = NULL};
	if (iof.fd[0] != -1)
		cmd[0] = get_command(argv[2], envp);
	i = 0;
	while (++i < argc - 3)
		cmd[i] = get_command(argv[i + 2], envp);
	if (cmd[i - 1].status)
	{
		i = cmd[i - 1].status;
		free_command_vector(argc - 3, &cmd);
		exit(i);
	}
	return (cmd);
}

static void	pipex(t_fd_pair iof, t_command *cmd, char *envp[])
{
	t_fd_pair	channel;
	pid_t		pid;

	channel.status = pipe(channel.fd);
	if (channel.status)
		print_error_exit("pipex: pipe");
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		close(channel.fd[0]);
		exec_cmd(iof.fd[0], channel.fd[1], &cmd[0], envp);
	}
	else
	{
		if (waitpid(pid, NULL, 0) == -1)
			print_error_exit("pipex: waitpid");
		close(channel.fd[1]);
		exec_cmd(channel.fd[0], iof.fd[1], &cmd[1], envp);
	}
}

static void	exec_cmd(int r_fd, int w_fd, t_command *cmd, char *envp[])
{
	dup2(r_fd, STDIN_FILENO);
	dup2(w_fd, STDOUT_FILENO);
	if (cmd->status == 0)
	{
		execve(cmd->pathname, cmd->param, envp);
		perror("pipex: execve");
	}
}
