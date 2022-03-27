/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idk.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:00:24 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/27 16:15:39 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// char	*read_stuff(int fd)
// {
// 	char	*temp;
// 	char	*line;
// 	char	*result;

// 	while (line = get_next_line(fd))
// 	{
// 		result = ft_strjoin(temp, line)
// 	}
// }









int	recursive_pipex(t_fd_pair channel, t_command *cmd, int cmd_count,
			char *envp[])
{
	pid_t	pid;
	int		r_fd;
	int		w_fd;

	r_fd = dup(channel.fd[0]);
	w_fd = channel.fd[1];
	close_fd_pair(channel);
	if (r_fd == -1)
		print_error_exit("pipex: dup");
	if (pipe(channel.fd))
		print_error_exit("pipex: pipe");
	pid = fork();
	if (pid == -1)
		print_error_exit("pipex: fork");
	if (pid == 0)
	{
		close(channel.fd[0]); //close read
		return (exec_cmd(r_fd, channel.fd[1], cmd[0], envp));
		//				read		write		cmd		envp
	}
	else
	{
		if (waitpid(pid, NULL, 0) == -1)
			perror("pipex: waitpid");
		// duplica o pipe-read e fecha o pipe
		close(channel.fd[1]);
		if (cmd_count > 1)
		{
			channel.fd[1] = w_fd;
			return (recursive_pipex(channel, cmd + 1, cmd_count - 1, envp));
		}
		if (w_fd != -1)
			return (exec_cmd(channel.fd[0], w_fd, cmd[0], envp));
	}
	return (EXIT_FAILURE);
}

// pipe0 pipe1
// cmd0 iof		pipe0	5
// cmd1 pipe0 	pipe1	4
// cmd2 pipe1	pipe0	3
// cmd3 pipe0 	pipe1	2
// cmdf pipe01	iof		1

//
