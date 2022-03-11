/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:14:13 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/11 19:26:10 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>	// access, dup, dup2, execve, fork, pipe, unlink
# include <fcntl.h>		// open
# include <stdlib.h>	// exit, //free, malloc
# include <string.h>	// strerror
# include <sys/wait.h>	// wait, waitpid
# include "not_libft.h"
# include "define.h"

# include <stdio.h>
typedef struct s_command
{
	char	*pathname;
	char	**param;
	int		param_count;
}	t_command;

// command.c
t_command	get_command(char *arg, char *envp[]);

// verifier.c
int		param_verifier(const int argc);
int		path_verifier(t_command *cmd, char ***paths);
int		pathname_verifier(t_command *cmd);

// error.c
void	print_coded_error(char *program, char *err_location, int err_code);
void	print_custom_error(char *program, char *err_location, char *err_msg);

#endif
