/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dismount.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 01:00:12 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/24 02:00:23 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_if_valid_fd(int fd)
{
	if (fd != -1)
		close(fd);
}

void	close_fd_pair(t_fd_pair fd_pair)
{
	close_if_valid_fd(fd_pair.fd[0]);
	close_if_valid_fd(fd_pair.fd[1]);
}

void	free_command(t_command *cmd)
{
	if (cmd->param)
		ft_free_char_matrix(&cmd->param);
	if (cmd->pathname)
		ft_null_free((void *)(&cmd->pathname));
}

void	free_command_vector(int argc, t_command **cmd)
{
	int	i;

	if (!*cmd)
		return ;
	i = 0;
	while (i < argc)
	{
		free_command(*cmd + i);
		i++;
	}
	ft_null_free((void *)(cmd));
}
