/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:40 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/29 03:44:03 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_fd_pair	io_file_opener(char *infilename, char *outfilename,
						int here_doc);
static int			exec_cmd(int r_fd, int w_fd, t_command cmd, char *envp[]);
static int			recursive_pipex(t_fd_pair channel, t_command *cmd,
						int cmd_count, char *envp[]);
static int			read_input(int i_fd, char *limiter);

int	main(int argc, char *argv[], char *envp[])
{
	t_command	*cmd;
	t_fd_pair	iof;
	int			here_doc;
	int			cmd_count;
	int			status;

	status = EXIT_FAILURE;
	if (param_verifier(argc) == EXIT_FAILURE)
		return (status);
	here_doc = !ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1]));
	iof = io_file_opener(argv[1], argv[argc - 1], here_doc);
	if (here_doc)
	{
		cmd_count = argc - 4;
		iof.fd[0] = read_input(iof.fd[0], argv[2]);
	}
	else
		cmd_count = argc - 3;
	cmd = fetch_commands(cmd_count, argv + 2 + here_doc, envp, iof);
	status = recursive_pipex(iof, cmd, cmd_count, envp);
	free_command_vector(cmd_count, &cmd);
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

static int	read_input(int i_fd, char *limiter)
{
	char	*temp;
	int		magic_pipe[2];

	temp = NULL;
	if (pipe(magic_pipe))
		print_error_exit("pipex: pipe");
	while (1)
	{
		temp = get_next_line(i_fd);
		if (temp && ft_strlen(temp) == ft_strlen(limiter) + 1
			&& ft_strncmp(temp, limiter, ft_strlen(temp) - 1) == 0
			&& temp[ft_strlen(temp) - 1] == '\n')
			break ;
		if (temp)
			ft_putstr_fd(temp, magic_pipe[1]);
		ft_null_free((void *)&temp);
	}
	close(magic_pipe[1]);
	return (magic_pipe[0]);
}

static int	recursive_pipex(t_fd_pair channel, t_command *cmd, int cmd_count,
				char *envp[])
{
	pid_t	pid;
	int		r_fd;
	int		o_fd;

	o_fd = channel.fd[1];
	r_fd = dup(channel.fd[0]);
	close_if_valid_fd(channel.fd[0]);
	if (pipe(channel.fd))
		print_error_exit("pipex: pipe");
	pid = fork();
	if (pid == -1)
		print_error_exit("pipex: fork");
	if (pid == 0)
		close_2_fds(o_fd, channel.fd[0]);
	if (pid == 0)
		return (exec_cmd(r_fd, channel.fd[1], cmd[0], envp));
	close_2_fds(r_fd, channel.fd[1]);
	if (waitpid(pid, NULL, 0) == -1)
		print_error_exit("pipex: waitpid");
	channel.fd[1] = o_fd;
	if (cmd_count > 2)
		return (recursive_pipex(channel, cmd + 1, cmd_count - 1, envp));
	if (o_fd != -1)
		return (exec_cmd(channel.fd[0], o_fd, cmd[1], envp));
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
