/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 18:16:29 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/31 01:30:16 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

typedef struct s_command
{
	char	*pathname;
	char	**param;
	int		status;
}	t_command;

typedef struct s_fd_pair
{
	int	fd[2];
}	t_fd_pair;

// Enviroment variable which contains path
# define ENV_PATH_VAR	"PATH="

// errno.h error codes
# define ENOENT		2	// No such file or directory
# define EACCESS	13	// Permission denied
# define EINVAL		22	// Invalid argument

// Porgram exit status / bash error codes
# define CMD_NOT_EXEC	126	// Command invoked cannot execute
# define CMD_NOT_FOUND	127	// command not found

#endif
