/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 18:16:29 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/17 20:24:03 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

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
