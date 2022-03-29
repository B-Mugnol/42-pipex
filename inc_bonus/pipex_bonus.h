/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:13 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/29 06:25:52 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>	// access, dup, dup2, execve, fork, pipe
# include <fcntl.h>		// open
# include <stdlib.h>	// malloc
# include <stdio.h>		// perror
# include <string.h>	// strerror
# include <sys/wait.h>	// wait, waitpid
# include "libft.h"
# include "define_bonus.h"

// command.c
t_command	*fetch_commands(int cmd_count, char *argv[], char *envp[],
				t_fd_pair iof);

// dismount.c
void		close_if_valid_fd(int fd);
void		close_fd_pair(t_fd_pair iof);
void		close_2_fds(int fd1, int fd2);
void		free_command(t_command *cmd);
void		free_command_vector(int argc, t_command **cmd);

// verifier.c
int			param_verifier(const int argc);
int			path_verifier(char **paths);
int			pathname_verifier(t_command cmd);

// error.c
void		print_coded_error(char *program, char *err_location, int err_code);
void		print_custom_error(char *program, char *err_location,
				char *err_msg);
int			print_error(char *err_location);
void		print_error_terminate(char *err_location, t_fd_pair iof);

#endif
