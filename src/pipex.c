/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:40 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/24 17:09:04 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_fd_pair	io_file_opener(char *infilename, char *outfilename);
// static t_command	*fetch_commands(char *argv[], char *envp[],	t_fd_pair iof);
static void			exec_cmd(int r_fd, int w_fd, t_command cmd, char *envp[]);
static void			pipex(t_fd_pair iof, t_command *cmd, char *envp[]);

int	main(int argc, char *argv[], char *envp[])
{
	t_command	*cmd;
	t_fd_pair	iof;

	if (param_verifier(argc))
		return (EXIT_FAILURE);
	iof = io_file_opener(argv[1], argv[argc - 1]);
	cmd = fetch_commands(argv, envp, iof);
	pipex(iof, cmd, envp);
	free_command_vector(2, &cmd);
	close_fd_pair(iof);
	return (EXIT_FAILURE);
}

static t_fd_pair	io_file_opener(char *infilename, char *outfilename)
{
	t_fd_pair	f;

	f.fd[0] = open(infilename, O_RDONLY);
	if (f.fd[0] == -1 && access(infilename, F_OK) == -1)
		print_coded_error("pipex", infilename, ENOENT);
	else if (f.fd[0] == -1)
		print_coded_error("pipex", infilename, EACCESS);
	f.fd[1] = open(outfilename, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (f.fd[1] == -1)
		print_coded_error("pipex", outfilename, EACCESS);
	return (f);
}

static void	exec_cmd(int r_fd, int w_fd, t_command cmd, char *envp[])
{
	dup2(r_fd, STDIN_FILENO);
	close_if_valid_fd(r_fd);
	dup2(w_fd, STDOUT_FILENO);
	close_if_valid_fd(w_fd);
	if (cmd.status == 0)
	{
		execve(cmd.pathname, cmd.param, envp);
		perror("pipex: execve");
	}
}

// static t_command	*fetch_commands(char *argv[], char *envp[],
// 						t_fd_pair iof)
// {
// 	t_command	*cmd;
// 	int			status;

// 	cmd = malloc(2 * sizeof (t_command));
// 	cmd[0] = (t_command){.status = -1, .param = NULL, .pathname = NULL};
// 	cmd[1] = (t_command){.status = -1, .param = NULL, .pathname = NULL};
// 	if (iof.fd[0] != -1)
// 		cmd[0] = get_command(argv[2], envp);
// 	cmd[1] = get_command(argv[3], envp);
// 	if (cmd[1].status)
// 	{
// 		status = cmd[1].status;
// 		free_command_vector(2, &cmd);
// 		close_fd_pair(iof);
// 		exit(status);
// 	}
// 	return (cmd);
// }

static void	pipex(t_fd_pair iof, t_command *cmd, char *envp[])
{
	t_fd_pair	channel;
	pid_t		pid;

	if (pipe(channel.fd))
		print_error_exit("pipex: pipe");
	pid = fork();
	if (pid == -1)
		return ;
	if (pid == 0)
	{
		close(channel.fd[0]);
		close_if_valid_fd(iof.fd[1]);
		if (iof.fd[0] != -1 && cmd[0].status == 0)
			exec_cmd(iof.fd[0], channel.fd[1], cmd[0], envp);
	}
	else
	{
		if (waitpid(pid, NULL, 0) == -1)
			print_error_exit("pipex: waitpid");
		close(channel.fd[1]);
		close_if_valid_fd(iof.fd[0]);
		if (iof.fd[1] != -1 && cmd[1].status == 0)
			exec_cmd(channel.fd[0], iof.fd[1], cmd[1], envp);
	}
}
