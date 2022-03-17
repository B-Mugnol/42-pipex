/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifier.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:25:33 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/14 20:02:15 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	param_verifier(const int argc)
{
	if (argc != 5)
	{
		print_coded_error("pipex", NULL, EINVAL);
		return (1);
	}
	return (0);
}

int	path_verifier(char **paths)
{
	if (paths == NULL)
	{
		print_coded_error("pipex", "envp", EINVAL);
		return (1);
	}
	return (0);
}

int	pathname_verifier(t_command cmd)
{
	if (!cmd.pathname)
	{
		print_custom_error("pipex", cmd.param[0], "command not found");
		return (CMD_NOT_FOUND);
	}
	else if (access(cmd.pathname, R_OK))
	{
		print_custom_error("pipex", cmd.pathname, "Permission denied");
		return (CMD_NOT_EXEC);
	}
	return (0);
}
