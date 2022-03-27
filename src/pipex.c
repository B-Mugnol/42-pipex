/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:40 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/27 20:34:22 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_fd_pair	io_file_opener(char *infilename, char *outfilename,
						int here_doc);
static int			exec_cmd(int r_fd, int w_fd, t_command cmd, char *envp[]);
static int			pipex(t_fd_pair iof, t_command *cmd, char *envp[],
						int cmd_count);

int	main(int argc, char *argv[], char *envp[])
{
	t_command	*cmd;
	t_fd_pair	iof;
	int			here_doc;
	int			status;

	status = EXIT_FAILURE;
	if (param_verifier(argc) == EXIT_FAILURE)
		return (status);
	here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
	iof = io_file_opener(argv[1], argv[argc - 1], here_doc);
	cmd = fetch_commands(argc, argv, envp, iof);
	status = pipex(iof, cmd, envp, argc - 3 - here_doc);
	free_command_vector(argc - 3, &cmd);
	close_fd_pair(iof);
	return (status);
}

static t_fd_pair	io_file_opener(char *infilename, char *outfilename,
						int here_doc)
{
	t_fd_pair	f;

	if (here_doc)
		f.fd[0] = STDIN_FILENO;
	else
	{
		f.fd[0] = open(infilename, O_RDONLY);
		if (f.fd[0] == -1 && access(infilename, F_OK) == -1)
			print_coded_error("pipex", infilename, ENOENT);
		else if (f.fd[0] == -1)
			print_coded_error("pipex", infilename, EACCESS);
	}
	if (here_doc)
		f.fd[1] = open(outfilename, O_WRONLY | O_APPEND | O_CREAT, 0664);
	else
		f.fd[1] = open(outfilename, O_WRONLY | O_TRUNC | O_CREAT, 0664);
	if (f.fd[1] == -1)
		print_coded_error("pipex", outfilename, EACCESS);
	return (f);
}

// static void	pipex(t_fd_pair iof, t_command *cmd, char *envp[], int cmd_count)
// {
// 	t_fd_pair	channel[2];
// 	pid_t		pid;

// 	if (pipe(channel[0].fd))
// 		print_error_exit("pipex: pipe");
// 	pid = fork();
// 	if (pid == -1)
// 		print_error_exit("pipex: fork");
// 	if (pid == 0)
// 	{
// 		close(channel[0].fd[0]);
// 		exec_cmd(iof.fd[0], channel[0].fd[1], cmd[0], envp);
// 	}
// 	else
// 	{
// 		if (waitpid(pid, NULL, 0) == -1)
// 			perror("pipex: waitpid");
// 		close(channel[0].fd[1]);
// 		exec_cmd(channel[0].fd[0], iof.fd[1], cmd[1], envp);
// 	}
// }

static int	pipex(t_fd_pair iof, t_command *cmd, char *envp[], int cmd_count)
{
	return (recursive_pipex(iof, cmd, cmd_count, envp));
	// t_fd_pair	channel[2];
	// pid_t		pid;
	// //outside function
	// if (pipe(channel[0].fd))
	// 	print_error_exit("pipex: pipe");
	// if (cmd_count > 2 && pipe(channel[1].fd))
	// {
	// 	close(channel[0].fd[0]);
	// 	close(channel[0].fd[1]);
	// 	print_error_exit("pipex: pipe");
	// }
	// //
	// pid = fork();
	// if (pid == -1)
	// 	print_error_exit("pipex: fork");
	// if (pid == 0)
	// {
	// 	close(channel[0].fd[0]);
	// 	exec_cmd(iof.fd[0], channel[0].fd[1], cmd[0], envp);
	// }
	// else
	// {
	// 	if (waitpid(pid, NULL, 0) == -1)
	// 		perror("pipex: waitpid");
	// 	if (--cmd_count > 1)//
	// 		// recursive_pipex();
	// 	close(channel[0].fd[1]);
	// 	exec_cmd(channel[0].fd[0], iof.fd[1], cmd[1], envp);
	// }
}

int	recursive_pipex(t_fd_pair channel, t_command *cmd, int cmd_count, char *envp[])
{
	pid_t	pid;
	int		r_fd;
	int		w_fd;

	w_fd = channel.fd[1];
	r_fd = dup(channel.fd[0]);
	close_if_valid_fd(channel.fd[0]);
	// if (r_fd == -1)
	// 	print_error_exit("pipex: dup");
	if (pipe(channel.fd))
		print_error_exit("pipex: pipe");
	pid = fork();
	if (pid == -1)
		print_error_exit("pipex: fork");
	if (pid == 0)
	{
		close(channel.fd[0]); //close read
		return (exec_cmd(r_fd, channel.fd[1], cmd[0], envp));
	}
	else
	{
		if (waitpid(pid, NULL, 0) == -1)
			perror("pipex: waitpid");
		// duplica o pipe-read e fecha o pipe
		close(channel.fd[1]);
		channel.fd[1] = w_fd;
		if (cmd_count > 1)
			return (recursive_pipex(channel, cmd + 1, cmd_count - 1, envp));
		if (w_fd != -1)
			return (exec_cmd(channel.fd[0], w_fd, cmd[0], envp));
	}
	return (EXIT_FAILURE);
}

static int	exec_cmd(int r_fd, int w_fd, t_command cmd, char *envp[])
{
	dup2(r_fd, STDIN_FILENO);
	close_if_valid_fd(r_fd);
	dup2(w_fd, STDOUT_FILENO);
	close_if_valid_fd(w_fd);
	if (cmd.status == 0)
	{
		execve(cmd.pathname, cmd.param, envp);
		perror("pipex: execve");
		return (EXIT_FAILURE);
	}
	else
		return (cmd.status);
}
