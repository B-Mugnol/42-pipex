/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:40 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/17 20:31:16 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_fd_pair	io_file_opener(char *infilename, char *outfilename);
static t_command	*fetch_commands(char *argv[], char *envp[],	t_fd_pair iof);
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
	cmd = fetch_commands(argv, envp, iof);
	pipex(iof, cmd, envp);
	free_command(&cmd[0]);
	free_command(&cmd[1]);
	free(cmd);
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

static void	exec_cmd(int r_fd, int w_fd, t_command *cmd, char *envp[])
{
	dup2(r_fd, STDIN_FILENO);
	dup2(w_fd, STDOUT_FILENO);
	if (cmd->status == 0)
		execve(cmd->pathname, cmd->param, envp);
	perror("pipex: execve");
}

static t_command	*fetch_commands(char *argv[], char *envp[],
						t_fd_pair iof)
{
	t_command	*cmd;

	cmd = malloc(2 * sizeof (t_command));
	cmd[0] = (t_command){.status = -1};
	cmd[1] = (t_command){.status = -1};
	if (iof.fd[0] != -1)
		cmd[0] = get_command(argv[2], envp);
	cmd[1] = get_command(argv[3], envp);
	if (cmd[0].status)
		free_command(&cmd[0]);
	if (cmd[1].status)
	{
		free_command(&cmd[0]);
		free_command(&cmd[1]);
		free(cmd);
		exit(cmd[1].status);
	}
	return (cmd);
}

static void	pipex(t_fd_pair iof, t_command *cmd, char *envp[])
{
	t_fd_pair	channel;
	pid_t		pid;

	channel.status = pipe(channel.fd);
	if (channel.status)
	{
		perror("pipex: pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == 0)
	{
		close(channel.fd[0]);
		exec_cmd(iof.fd[0], channel.fd[1], &cmd[0], envp);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(channel.fd[1]);
		exec_cmd(channel.fd[0], iof.fd[1], &cmd[1], envp);
	}
}
