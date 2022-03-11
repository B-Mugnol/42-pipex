/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verifier.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/06 19:25:33 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/11 19:25:38 by bmugnol-         ###   ########.fr       */
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

int	path_verifier(t_command *cmd, char ***paths)
{
	if (paths == NULL)
	{
		print_coded_error("pipex", "envp", EINVAL);
		ft_free_matrix((void *)(&cmd->param), cmd->param_count);
		cmd->param_count = 0;
		return (1);
	}
	return (0);
}

int	pathname_verifier(t_command *cmd)
{
	if (!cmd->pathname)
	{
		print_custom_error("pipex", cmd->param[0], "Command does not exist");
		ft_free_matrix((void *)(&cmd->param), cmd->param_count);
		cmd->param_count = 0;
		return (1);
	}
	else if (access(cmd->pathname, R_OK))
	{
		print_custom_error("pipex", cmd->pathname, "Permission denied");
		ft_null_free((void *)(&cmd->pathname));
		ft_free_matrix((void *)(&cmd->param), cmd->param_count);
		cmd->param_count = 0;
		return (1);
	}
	return (0);
}
