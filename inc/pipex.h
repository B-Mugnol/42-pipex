/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:13 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/17 21:16:41 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>	// access, dup, dup2, execve, fork, pipe
# include <fcntl.h>		// open
# include <stdlib.h>	// exit, malloc
# include <stdio.h>		// perror
# include <string.h>	// strerror
# include <sys/wait.h>	// wait, waitpid
# include "libft.h"
# include "define.h"

typedef struct s_command
{
	char	*pathname;
	char	**param;
	int		status;
}	t_command;

typedef struct s_fd_pair
{
	int	fd[2];
	int	status;
}	t_fd_pair;

// command.c
t_command	get_command(char *arg, char *envp[]);
void		free_command(t_command *cmd);

// verifier.c
int			param_verifier(const int argc);
int			path_verifier(char **paths);
int			pathname_verifier(t_command cmd);

// error.c
void		print_coded_error(char *program, char *err_location, int err_code);
void		print_custom_error(char *program, char *err_location,
				char *err_msg);
void		print_error_exit(char *err_location);

#endif
